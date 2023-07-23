#ifndef GAMEENGINE_HPP
#define GAMEENGINE_HPP

#include "Tile.hpp"
#include "RandomNumberGenerator.hpp"
#include "GameState.hpp"

#include <vector>
#include <queue>
#include <algorithm>

class GameEngine
{
    public:
        GameEngine();
        virtual ~GameEngine();

        void startNewGame(const int, const int, const int);
        void processMove(const int, const int);
        void increaseTimer();

        const std::vector <std::vector <Tile>>& getTileMap() const;
        int getScore() const;
        int getTimeInSeconds() const;

        int getTileMapWidth() const;
        int getTileMapHeight() const;

    private:
        std::vector <std::vector <Tile>> m_tileMap;
        std::pair <int, int> m_selection;
        GameState m_state;

        bool m_isAdditionalMoveAvailable;
        int m_timeElapsedInSeconds;
        int m_score;

        int m_colorCount;
        int m_minStreakLength;

        RandomNumberGenerator m_random;

        const int m_newBallCountOnMove = 3;

        void selectTile(const int, const int);
        void deselectTile();
        void moveSelectedToTile(const int, const int);

        void addExpectedBalls(const int);
        void transformExpectedBalls();

        bool isTilePassable(const int, const int) const;
        bool pathExists(const int, const int, const int, const int) const;

        void deleteStreaks(const int, const int, bool);

        bool isHorizontalStreak(const int, const int) const;
        int deleteAdjacentHorizontalStreak(const int, const int);

        bool isVerticalStreak(const int, const int) const;
        int deleteAdjacentVerticalStreak(const int, const int);

        bool isMainDiagonalStreak(const int, const int) const;
        int deleteAdjacentMainDiagonalStreak(const int, const int);

        bool isAntiDiagonalStreak(const int, const int) const;
        int deleteAdjacentAntiDiagonalStreak(const int, const int);

        void increaseScore(const int);
};

#endif // GAMEENGINE_HPP
