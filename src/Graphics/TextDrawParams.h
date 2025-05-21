#pragma once

namespace Minecraft
{
    struct TextDrawParams
    {
        // TODO make these colours
        vec3 TextColor = vec3(1);
        optional<vec3> ShadowColor = nullopt;
        bool HasShadow = true;
        // TODO: do we want position + origin? maybe origin? idk
    };
}
