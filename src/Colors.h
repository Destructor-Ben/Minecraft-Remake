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

    // TODO: redo these, load from config file

    inline const Color RarityRed = Color::FromHex("#ff5959").value();
    inline const Color RarityOrange = Color::FromHex("#ffa459").value();
    inline const Color RarityYellow = Color::FromHex("#ffec59").value();
    inline const Color RarityGreen = Color::FromHex("#59ff5f").value();
    inline const Color RarityCyan = Color::FromHex("#59f9ff").value();
    inline const Color RarityBlue = Color::FromHex("#596cff").value();
    inline const Color RarityPurple = Color::FromHex("#c859ff").value();
    inline const Color RarityPink = Color::FromHex("#ff59f7").value();

    #pragma endregion
}
