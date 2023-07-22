#ifndef TILE_HPP
#define TILE_HPP

#include <string>
#include <stdexcept>

enum class Tile
{
    Empty,

    ColorOne,
    ColorTwo,
    ColorThree,
    ColorFour,
    ColorFive,
    ColorSix,
    ColorSeven,
    ColorEight,
    ColorEnd,

    ExpectedColorOne,
    ExpectedColorTwo,
    ExpectedColorThree,
    ExpectedColorFour,
    ExpectedColorFive,
    ExpectedColorSix,
    ExpectedColorSeven,
    ExpectedColorEight,
    ExpectedColorEnd,

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

inline Tile operator++(Tile& x, int)
{
    x = static_cast <Tile>(static_cast <int>(x) + 1);
    return x;
}

inline Tile operator+(Tile x, Tile y)
{
    return static_cast <Tile>(static_cast <int>(x) + static_cast <int>(y));
}

inline Tile operator-(Tile x, Tile y)
{
    return static_cast <Tile>(static_cast <int>(x) - static_cast <int>(y));
}

inline Tile expectedToNormal(Tile x)
{
    return x - Tile::ColorEnd;
}

inline Tile normalToExpected(Tile x)
{
    return x + Tile::ColorEnd;
}

inline Tile selectedToNormal(Tile x)
{
    return x - Tile::ExpectedColorEnd;
}

inline Tile normalToSelected(Tile x)
{
    return x + Tile::ExpectedColorEnd;
}

#endif // TILE_HPP
