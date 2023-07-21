#ifndef RANDOMNUMBERGENERATOR_HPP
#define RANDOMNUMBERGENERATOR_HPP

#include "Tile.hpp"

#include <chrono>
#include <random>

class RandomNumberGenerator
{
    public:
        RandomNumberGenerator();
        virtual ~RandomNumberGenerator();

        int getInteger(const int, const int);
        Tile getTile(const Tile, const Tile);

    private:
        std::mt19937 m_engine;
};

#endif // RANDOMNUMBERGENERATOR_HPP
