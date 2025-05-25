#pragma once

#include "Color.h"

namespace Minecraft::Colors
{
    #pragma region Regular Colors

    inline const Color White = Color(1, 1, 1);
    inline const Color Black = Color(0, 0, 0);
    inline const Color Transparent = Color(0, 0, 0, 0);
    inline const Color Red = Color(1, 0, 0);
    inline const Color Green = Color(0, 1, 0);
    inline const Color Blue = Color(0, 0, 1);
    inline const Color Cyan = Color(1, 1, 0);
    inline const Color Magenta = Color(1, 0, 1);
    inline const Color Yellow = Color(0, 1, 1);

    #pragma endregion

    #pragma region Rarity Colors

    inline const Color RarityRed = Color::FromHex("#ff5959");
    inline const Color RarityOrange = Color::FromHex("#ffa459");
    inline const Color RarityYellow = Color::FromHex("#ffec59");
    inline const Color RarityGreen = Color::FromHex("#59ff5f");
    inline const Color RarityCyan = Color::FromHex("#59f9ff");
    inline const Color RarityBlue = Color::FromHex("#596cff");
    inline const Color RarityPurple = Color::FromHex("#c859ff");
    inline const Color RarityPink = Color::FromHex("#ff59f7");

    #pragma endregion
}
