#ifndef TILE_HPP
#define TILE_HPP

#include <string>
#include <stdexcept>

enum class Tile
{
    Empty,

    ColorStart,
    ColorOne,
    ColorTwo,
    ColorThree,
    ColorFour,
    ColorFive,
    ColorSix,
    ColorSeven,
    ColorEight,
    ColorEnd,

    ExpectedColorStart,
    ExpectedColorOne,
    ExpectedColorTwo,
    ExpectedColorThree,
    ExpectedColorFour,
    ExpectedColorFive,
    ExpectedColorSix,
    ExpectedColorSeven,
    ExpectedColorEight,
    ExpectedColorEnd,

    SelectedColorStart,
    SelectedColorOne,
    SelectedColorTwo,
    SelectedColorThree,
    SelectedColorFour,
    SelectedColorFive,
    SelectedColorSix,
    SelectedColorSeven,
    SelectedColorEight,
    SelectedColorEnd,

    Count
};

inline Tile operator++(Tile x, int)
{
    return static_cast <Tile>(static_cast <int>(x) + 1);
}

inline Tile operator+(Tile x, Tile y)
{
    return static_cast <Tile>(static_cast <int>(x) + static_cast <int>(y));
}

inline Tile operator-(Tile x, Tile y)
{
    return static_cast <Tile>(static_cast <int>(x) - static_cast <int>(y));
}

#endif // TILE_HPP
