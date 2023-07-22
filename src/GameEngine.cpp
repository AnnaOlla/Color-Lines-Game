#include "GameEngine.hpp"

GameEngine::GameEngine()
{
    //ctor
}

GameEngine::~GameEngine()
{
    //dtor
}

void GameEngine::startNewGame(const int widthInTiles, const int heightInTiles, const int colorCount)
{
    m_tileMap.resize(heightInTiles, std::vector <Tile>(widthInTiles));
    for (auto& row : m_tileMap)
        std::fill(row.begin(), row.end(), Tile::Empty);

    m_selection = std::make_pair(-1, -1);
    m_colorCount = colorCount;
    m_minStreakLength = (widthInTiles < heightInTiles) ? (widthInTiles - 4) : (heightInTiles - 4);
    m_state = GameState::FirstPick;

    addExpectedBalls(m_newBallCountOnMove);
    transformExpectedBalls();
    addExpectedBalls(m_newBallCountOnMove);
}

void GameEngine::processMove(const int row, const int column)
{
    switch (m_state)
    {
    case GameState::FirstPick:
        selectTile(row, column);
        break;

    case GameState::SecondPick:
        if (m_selection == std::make_pair(row, column))
            deselectTile();
        else
            moveSelectedToTile(row, column);
        break;

    default:
        break;
    }
}

void GameEngine::selectTile(const int row, const int column)
{
    if (m_state != GameState::FirstPick)
        return;

    if (m_tileMap[row][column] < Tile::ColorOne || m_tileMap[row][column] >= Tile::ColorEnd)
        return;

    m_tileMap[row][column] = m_tileMap[row][column] + Tile::ExpectedColorEnd;
    m_selection = std::make_pair(row, column);
    m_state = GameState::SecondPick;
}

void GameEngine::deselectTile()
{
    if (m_state != GameState::SecondPick)
        return;

    auto row = m_selection.first;
    auto column = m_selection.second;

    if (row == -1 || column == -1)
        return;

    m_tileMap[row][column] = m_tileMap[row][column] - Tile::ExpectedColorEnd;
    m_selection = std::make_pair(-1, -1);
    m_state = GameState::FirstPick;
}

void GameEngine::moveSelectedToTile(const int rowNew, const int columnNew)
{
    auto rowOld = m_selection.first;
    auto columnOld = m_selection.second;

    std::vector <std::pair <int, int>> path;
    if (!findPath(m_selection.first, m_selection.second, rowNew, columnNew, path))
        return;

    std::swap(m_tileMap[rowNew][columnNew], m_tileMap[rowOld][columnOld]);
    m_selection = std::make_pair(rowNew, columnNew);
    deselectTile();

    m_isAdditionalMoveAvailable = false;
    deleteStreaks(rowNew, columnNew, true);

    if (!m_isAdditionalMoveAvailable)
    {
        transformExpectedBalls();
        addExpectedBalls(m_newBallCountOnMove);
    }
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

    const int count = (maxCount > static_cast <int>(emptyTiles.size())) ? static_cast <int>(emptyTiles.size()) : maxCount;

    for (auto i = 0; i < count; )
    {
        auto index = random.getInteger(0, emptyTiles.size());

        auto row = emptyTiles[index].first;
        auto column = emptyTiles[index].second;

        if (m_tileMap[row][column] != Tile::Empty)
            continue;

        m_tileMap[row][column] = random.getTile(Tile::ExpectedColorOne,
                                                Tile::ExpectedColorOne + static_cast <Tile>(m_colorCount));
        i++;
    }
}

void GameEngine::transformExpectedBalls()
{
    for (auto row = 0; row < m_tileMap.size(); row++)
    {
        for (auto column = 0; column < m_tileMap.size(); column++)
        {
            if (m_tileMap[row][column] >= Tile::ExpectedColorOne && m_tileMap[row][column] < Tile::ExpectedColorEnd)
            {
                m_tileMap[row][column] = expectedToNormal(m_tileMap[row][column]);
                deleteStreaks(row, column, false);
            }
        }
    }
}

bool GameEngine::isTilePassable(const int row, const int column) const
{
    const auto t = m_tileMap[row][column];
    return (t == Tile::Empty || (t >= Tile::ExpectedColorOne && t < Tile::ExpectedColorEnd));
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

        if (nextRow    >= 0 && nextRow    < m_tileMap.size()    &&
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

void GameEngine::deleteStreaks(const int row, const int column, const bool playerActed)
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

    increaseScore(totalStreakLength);

    if (playerActed)
        m_isAdditionalMoveAvailable = true;
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

    for (auto i = row + 1, j = column - 1; i < m_tileMap.size() && j >= 0; i++, j--)
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

    for (auto i = row + 1, j = column - 1; i < m_tileMap.size() && j >= 0; i++, j--)
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

void GameEngine::increaseScore(const int streakLength)
{
    m_score += streakLength * (streakLength - m_minStreakLength + 1);
}

const std::vector <std::vector <Tile>>& GameEngine::getTileMap() const
{
    return m_tileMap;
}

int GameEngine::getScore() const
{
    return m_score;
}

void GameEngine::increaseTimer()
{
    m_timeElapsedInSeconds++;
}

int GameEngine::getTimeInSeconds() const
{
    return m_timeElapsedInSeconds;
}

int GameEngine::getTileMapWidth() const
{
    return m_tileMap[0].size();
}

int GameEngine::getTileMapHeight() const
{
    return m_tileMap.size();
}
