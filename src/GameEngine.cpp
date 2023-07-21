#include "GameEngine.hpp"

GameEngine::GameEngine()
{
    m_selection.first = -1;
    m_selection.second = -1;
}

GameEngine::~GameEngine()
{
    //dtor
}

bool GameEngine::selectTile(const int row, const int column)
{
    if (!(m_tileMap[row][column] > Tile::ColorStart && m_tileMap[row][column] < Tile::ColorEnd))
        return false;

    m_tileMap[row][column] = m_tileMap[row][column] + Tile::SelectedColorStart;
    m_selection.first = row;
    m_selection.second = column;

    return true;
}

bool GameEngine::deselectTile()
{
    auto row = m_selection.first;
    auto column = m_selection.second;

    if (row == -1 || column == -1)
        return false;

    m_tileMap[row][column] = m_tileMap[row][column] - Tile::SelectedColorStart;
    m_selection.first = -1;
    m_selection.second = -1;

    return true;
}

bool GameEngine::moveToTile(const int rowNew, const int columnNew)
{
    auto rowOld = m_selection.first;
    auto columnOld = m_selection.second;

    if (rowOld == -1 || columnOld == -1)
        return false;

    std::vector <std::pair <int, int>> path;
    if (!findPath(m_selection.first, m_selection.second, rowNew, columnNew, path))
        return false;

    m_tileMap[rowNew][columnNew] = m_tileMap[rowOld][columnOld];
    m_selection.first = rowNew;
    m_selection.second = columnNew;
    deselectTile();

    return true;
}

void GameEngine::addExpectedBalls(const int maxCount)
{
    std::vector <std::pair <int, int>> emptyTiles;
    emptyTiles.reserve(m_tileMap.size() * m_tileMap[0].size());

    for (auto row = 0; row < m_tileMap.size(); row++)
    {
        for (auto column = 0; column < m_tileMap[row].size(); column++)
        {
            if (m_tileMap[row][column] == Tile::Empty)
                emptyTiles.push_back(std::make_pair(row, column));
        }
    }

    const int count = (maxCount >= static_cast <int>(emptyTiles.size())) ? maxCount : static_cast <int>(emptyTiles.size());

    for (auto i = 0; i < count; )
    {
        auto index = random.getInteger(0, emptyTiles.size());

        auto row = emptyTiles[index].first;
        auto column = emptyTiles[index].second;

        if (m_tileMap[row][column] == Tile::Empty)
            continue;

        m_tileMap[row][column] = random.getTile(Tile::ExpectedColorOne, Tile::ExpectedColorEnd);
        i++;
    }
}

void GameEngine::transformExpectedBalls()
{
    for (auto row = 0; row < m_tileMap.size(); row++)
    {
        for (auto column = 0; column < m_tileMap.size(); column++)
        {
            if (m_tileMap[row][column] > Tile::ExpectedColorStart && m_tileMap[row][column] < Tile::ExpectedColorEnd)
            {
                m_tileMap[row][column] = m_tileMap[row][column] - Tile::ExpectedColorStart;
                deleteStreaks(row, column);
            }
        }
    }
}

bool GameEngine::isTilePassable(const int row, const int column) const
{
    const auto t = m_tileMap[row][column];
    return (t == Tile::Empty || (t > Tile::ExpectedColorStart && t < Tile::ExpectedColorEnd));
}

bool GameEngine::findPath(const int currentRow,
                          const int currentColumn,
                          const int destinationRow,
                          const int destinationColumn,
                          std::vector <std::pair <int, int>>& path) const
{
    path.push_back(std::make_pair(currentRow, currentColumn));

    if (destinationRow == currentRow && destinationColumn == currentColumn)
        return true;

    static const std::vector <int> rowOffset    {-1, 1,  0, 0};
    static const std::vector <int> columnOffset { 0, 0, -1, 1};

    for (auto i = 0; i < rowOffset.size(); i++)
    {
        const int nextRow = rowOffset[i] + currentRow;
        const int nextColumn = columnOffset[i] + currentColumn;

        if (nextRow    >= 0 && nextRow    < m_tileMap.size() &&
            nextColumn >= 0 && nextColumn < m_tileMap[0].size() &&
            isTilePassable(nextRow, nextColumn) &&
            std::find(path.cbegin(), path.cend(), std::make_pair(nextRow, nextColumn)) == path.cend() &&
            findPath(nextRow, nextColumn, destinationRow, destinationColumn, path))
        {
            return true;
        }
    }

    path.pop_back();

    return false;
}

void GameEngine::deleteStreaks(const int row, const int column)
{
    auto totalStreakLength = 0;

    if (isHorizontalStreak(row, column))
        totalStreakLength += deleteAdjacentHorizontalStreak(row, column);

    if (isVerticalStreak(row, column))
        totalStreakLength += deleteAdjacentVerticalStreak(row, column);

    if (isMainDiagonalStreak(row, column))
        totalStreakLength += deleteAdjacentMainDiagonalStreak(row, column);

    if (isAntiDiagonalStreak(row, column))
        totalStreakLength += deleteAdjacentAntiDiagonalStreak(row, column);

    if (totalStreakLength == 0)
        return;

    m_tileMap[row][column] = Tile::Empty;
    totalStreakLength++;

    addScores(totalStreakLength);
}

bool GameEngine::isHorizontalStreak(const int row, const int column) const
{
    auto streakLength = 1;

    for (auto j = column - 1; j >= 0; j--)
    {
        if (m_tileMap[row][j] == m_tileMap[row][column])
            streakLength++;
        else
            break;
    }

    for (auto j = column + 1; j < m_tileMap[0].size(); j++)
    {
        if (m_tileMap[row][j] == m_tileMap[row][column])
            streakLength++;
        else
            break;
    }

    if (streakLength < m_minStreakLength)
        return false;
    return true;
}

int GameEngine::deleteAdjacentHorizontalStreak(const int row, const int column)
{
    auto streakLength = 0;

    for (auto j = column - 1; j >= 0; j--)
    {
        if (m_tileMap[row][j] == m_tileMap[row][column])
        {
            m_tileMap[row][j] = Tile::Empty;
            streakLength++;
        }
        else
            break;
    }

    for (auto j = column + 1; j < m_tileMap[0].size(); j++)
    {
        if (m_tileMap[row][j] == m_tileMap[row][column])
        {
            m_tileMap[row][j] = Tile::Empty;
            streakLength++;
        }
        else
            break;
    }

    return streakLength;
}

bool GameEngine::isVerticalStreak(const int row, const int column) const
{
    auto streakLength = 1;

    for (auto i = row - 1; i >= 0; i--)
    {
        if (m_tileMap[i][column] == m_tileMap[row][column])
            streakLength++;
        else
            break;
    }

    for (auto i = row + 1; i < m_tileMap.size(); i++)
    {
        if (m_tileMap[i][column] == m_tileMap[row][column])
            streakLength++;
        else
            break;
    }

    if (streakLength < m_minStreakLength)
        return false;
    return true;
}

int GameEngine::deleteAdjacentVerticalStreak(const int row, const int column)
{
    auto streakLength = 0;

    for (auto i = row - 1; i >= 0; i--)
    {
        if (m_tileMap[i][column] == m_tileMap[row][column])
        {
            m_tileMap[i][column] = Tile::Empty;
            streakLength++;
        }
        else
            break;
    }

    for (auto i = row + 1; i < m_tileMap.size(); i++)
    {
        if (m_tileMap[i][column] == m_tileMap[row][column])
        {
            m_tileMap[i][column] = Tile::Empty;
            streakLength++;
        }
        else
            break;
    }

    return streakLength;
}

bool GameEngine::isMainDiagonalStreak(const int row, const int column) const
{
    auto streakLength = 1;

    for (auto i = row - 1, j = column - 1; i >= 0 && j >= 0; i--, j--)
    {
        if (m_tileMap[i][j] == m_tileMap[row][column])
            streakLength++;
        else
            break;
    }

    for (auto i = row + 1, j = column + 1; i < m_tileMap.size() && j < m_tileMap[0].size(); i++, j++)
    {
        if (m_tileMap[i][j] == m_tileMap[row][column])
            streakLength++;
        else
            break;
    }

    if (streakLength < m_minStreakLength)
        return false;
    return true;
}

int GameEngine::deleteAdjacentMainDiagonalStreak(const int row, const int column)
{
    auto streakLength = 0;

    for (auto i = row - 1, j = column - 1; i >= 0 && j >= 0; i--, j--)
    {
        if (m_tileMap[i][j] == m_tileMap[row][column])
        {
            m_tileMap[i][j] = Tile::Empty;
            streakLength++;
        }
        else
            break;
    }

    for (auto i = row + 1, j = column + 1; i < m_tileMap.size() && j < m_tileMap[0].size(); i++, j++)
    {
        if (m_tileMap[i][j] == m_tileMap[row][column])
        {
            m_tileMap[i][j] = Tile::Empty;
            streakLength++;
        }
        else
            break;
    }

    return streakLength;
}

bool GameEngine::isAntiDiagonalStreak(const int row, const int column) const
{
    auto streakLength = 1;

    for (auto i = row - 1, j = column + 1; i >= 0 && j < m_tileMap[0].size(); i--, j++)
    {
        if (m_tileMap[i][j] == m_tileMap[row][column])
            streakLength++;
        else
            break;
    }

    for (auto i = row - 1, j = column + 1; i < m_tileMap.size() && j >= 0; i++, j--)
    {
        if (m_tileMap[i][j] == m_tileMap[row][column])
            streakLength++;
        else
            break;
    }

    if (streakLength < m_minStreakLength)
        return false;
    return true;
}

int GameEngine::deleteAdjacentAntiDiagonalStreak(const int row, const int column)
{
    auto streakLength = 0;

    for (auto i = row - 1, j = column + 1; i >= 0 && j < m_tileMap[0].size(); i--, j++)
    {
        if (m_tileMap[i][j] == m_tileMap[row][column])
        {
            m_tileMap[i][j] = Tile::Empty;
            streakLength++;
        }
        else
            break;
    }

    for (auto i = row - 1, j = column + 1; i < m_tileMap.size() && j >= 0; i++, j--)
    {
        if (m_tileMap[i][j] == m_tileMap[row][column])
        {
            m_tileMap[i][j] = Tile::Empty;
            streakLength++;
        }
        else
            break;
    }

    return streakLength;
}

void GameEngine::addScores(const int streakLength)
{
    m_score += streakLength * (streakLength - m_minStreakLength + 1);
}
