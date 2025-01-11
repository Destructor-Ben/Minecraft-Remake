#ifdef CPPTRACE_GET_SYMBOLS_WITH_LIBDWARF

#include "symbols/symbols.hpp"

#include <cpptrace/basic.hpp>
#include "dwarf/resolver.hpp"
#include "utils/common.hpp"
#include "utils/utils.hpp"

#include <cstdint>
#include <cstdio>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>


namespace cpptrace {
namespace detail {
namespace libdwarf {
    std::unique_ptr<symbol_resolver> get_resolver_for_object(const std::string& object_path) {
        #if IS_APPLE
        // Check if dSYM exist, if not fallback to debug map
        if(!directory_exists(object_path + ".dSYM")) {
            return make_debug_map_resolver(object_path);
        }
        #endif
        return make_dwarf_resolver(object_path);
    }

    // not thread-safe, replies on caller to lock
    maybe_owned<symbol_resolver> get_resolver(const std::string& object_name) {
        // cache resolvers since objects are likely to be traced more than once
        static std::unordered_map<std::string, std::unique_ptr<symbol_resolver>> resolver_map;
        auto it = resolver_map.find(object_name);
        if(it != resolver_map.end()) {
            return it->second.get();
        } else {
            std::unique_ptr<symbol_resolver> resolver_object = get_resolver_for_object(object_name);
            if(get_cache_mode() == cache_mode::prioritize_speed) {
                // .emplace needed, for some reason .insert tries to copy <= gcc 7.2
                return resolver_map.emplace(object_name, std::move(resolver_object)).first->second.get();
            } else {
                // gcc 4 has trouble with automatic moves of locals here https://godbolt.org/z/9oWdWjbf8
                return maybe_owned<symbol_resolver>{std::move(resolver_object)};
            }
        }
    }

    // flatten trace with inlines
    std::vector<stacktrace_frame> flatten_inlines(std::vector<frame_with_inlines>& trace) {
        std::vector<stacktrace_frame> final_trace;
        for(auto& entry : trace) {
            // most recent call first
            if(!entry.inlines.empty()) {
                // insert in reverse order
                final_trace.insert(
                    final_trace.end(),
                    std::make_move_iterator(entry.inlines.rbegin()),
                    std::make_move_iterator(entry.inlines.rend())
                );
            }
            final_trace.push_back(std::move(entry.frame));
            if(!entry.inlines.empty()) {
                // rotate line info due to quirk of how dwarf stores this stuff
                // inclusive range
                auto begin = final_trace.end() - (1 + entry.inlines.size());
                auto end = final_trace.end() - 1;
                auto carry_line = end->line;
                auto carry_column = end->column;
                std::string carry_filename = std::move(end->filename);
                for(auto it = end; it != begin; it--) {
                    it->line = (it - 1)->line;
                    it->column = (it - 1)->column;
                    it->filename = std::move((it - 1)->filename);
                }
                begin->line = carry_line;
                begin->column = carry_column;
                begin->filename = std::move(carry_filename);
            }
        }
        return final_trace;
    }

    CPPTRACE_FORCE_NO_INLINE_FOR_PROFILING
    std::vector<stacktrace_frame> resolve_frames(const std::vector<object_frame>& frames) {
        std::vector<frame_with_inlines> trace(frames.size(), {null_frame, {}});
        // Locking around all libdwarf interaction per https://github.com/davea42/libdwarf-code/discussions/184
        // And also locking for interactions with get_resolver
        static std::mutex mutex;
        const std::lock_guard<std::mutex> lock(mutex);
        for(const auto& group : collate_frames(frames, trace)) {
            try {
                const auto& object_name = group.first;
                auto resolver = get_resolver(object_name);
                for(const auto& entry : group.second) {
                    const auto& dlframe = entry.first.get();
                    auto& frame = entry.second.get();
                    try {
                        frame = resolver->resolve_frame(dlframe);
                    } catch(...) {
                        frame.frame.raw_address = dlframe.raw_address;
                        frame.frame.object_address = dlframe.object_address;
                        frame.frame.filename = dlframe.object_path;
                        if(!should_absorb_trace_exceptions()) {
                            throw;
                        }
                    }
                }
            } catch(...) { // NOSONAR
                if(!should_absorb_trace_exceptions()) {
                    throw;
                }
            }
        }
        // fill in basic info for any frames where there were resolution issues
        for(std::size_t i = 0; i < frames.size(); i++) {
            const auto& dlframe = frames[i];
            auto& frame = trace[i];
            if(frame.frame == null_frame) {
                frame = {
                    {
                        dlframe.raw_address,
                        dlframe.object_address,
                        nullable<std::uint32_t>::null(),
                        nullable<std::uint32_t>::null(),
                        dlframe.object_path,
                        "",
                        false
                    },
                    {}
                };
            }
        }
        // flatten and finish
        return flatten_inlines(trace);
    }
}
}
}

#endif
