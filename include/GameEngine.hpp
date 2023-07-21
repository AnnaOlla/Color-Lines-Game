#ifndef GAMEENGINE_HPP
#define GAMEENGINE_HPP

#include "Tile.hpp"
#include "RandomNumberGenerator.hpp"

#include <vector>

class GameEngine
{
    public:
        GameEngine();
        virtual ~GameEngine();

        bool selectTile(const int, const int);
        bool deselectTile();
        bool moveToTile(const int, const int);

        void addExpectedBalls(const int);
        void transformExpectedBalls();


    private:
        std::vector <std::vector <Tile>> m_tileMap;
        std::pair <int, int> m_selection;

        bool m_isAdditionalMoveAvailable;
        int m_timeElapsedInSeconds;
        int m_score;

        int m_colorCount;
        int m_combinationLength;

        RandomNumberGenerator random;
};

#endif // GAMEENGINE_HPP
