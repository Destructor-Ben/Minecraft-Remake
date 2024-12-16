# To Do List

- Fix compiling for Oliver
    - Make a placeholder for `format()` since he doesn't have the correct library for it

```cpp
template<typename... _Args>
inline string format(string __fmt, _Args&& ... __args) { return "[FORMAT PLACEHOLDER]"; }
```

- Also fix `stacktrace` being nonexistent for me and him

- Make the libraries submodules instead of directly embedded
- Embed resources - use source generator https://stackoverflow.com/a/71906177/12259381
- Transparency & blending
- Implement an ECS for entities and blocks
- Entities
- Make all chunk loops that use radii circle instead of squares
- Maybe make chunk positions (origins) their center instead of corner
    - Blocks already are, also check that they are done correctly
- Why is it so laggy when under the world?
