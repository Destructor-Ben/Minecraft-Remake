#pragma once

#include "Logger.h"

#include <yaml-cpp/yaml.h>

namespace Minecraft
{
    class Color;

    // Safe, exception free wrapper struct
    struct Config
    {
        YAML::Node Root;

        template<typename T>
        optional<T> TryGetValue(const string& key)
        {
            try
            {
                std::stringstream paths(key);

                YAML::Node node = YAML::Clone(Root);
                string nextNodeName;
                while (std::getline(paths, nextNodeName, '/')) {
                    node = node[nextNodeName];
                }

                return node.as<T>();
            }
            catch (const std::exception& exception)
            {
                Logger::Warn(format("Error occured while fetching config value: {}", exception.what()));
                return nullopt;
            }
        }
    };
}

// Add support for serializing/deserializing various types
namespace YAML
{
    using namespace Minecraft;

    template<>
    struct convert<Color>
    {
        static Node encode(const Color& rhs);
        static bool decode(const Node& node, Color& rhs);
    };
}
