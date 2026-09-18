#include "Config.h"

#include "Color.h"
#include "Logger.h"

namespace YAML
{
    using namespace Minecraft;

    Node convert<Color>::encode(const Color& rhs)
    {
        Node node;
        node.push_back(rhs.ToHex(true));
        return node;
    }

    bool convert<Color>::decode(const Node& node, Color& rhs)
    {
        if (!node.IsScalar())
            return false;

        auto parsedValue = Color::FromHex(node.as<string>());
        if (!parsedValue)
            return false;

        rhs = parsedValue.value();
        return true;
    }
}

