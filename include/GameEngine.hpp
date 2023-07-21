#ifndef GAMEENGINE_HPP
#define GAMEENGINE_HPP

#include "Tile.hpp"
#include "RandomNumberGenerator.hpp"

#include <vector>
#include <algorithm>

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
        int m_minStreakLength;

        RandomNumberGenerator random;

        bool isTilePassable(const int, const int) const;
        bool findPath(const int, const int, const int, const int, std::vector <std::pair <int, int>>&) const;

        void deleteStreaks(const int, const int);

        bool isHorizontalStreak(const int, const int) const;
        int deleteAdjacentHorizontalStreak(const int, const int);

        bool isVerticalStreak(const int, const int) const;
        int deleteAdjacentVerticalStreak(const int, const int);

        bool isMainDiagonalStreak(const int, const int) const;
        int deleteAdjacentMainDiagonalStreak(const int, const int);

        bool isAntiDiagonalStreak(const int, const int) const;
        int deleteAdjacentAntiDiagonalStreak(const int, const int);

        void addScores(const int);
};

#endif // GAMEENGINE_HPP
