#include "GameEngine.hpp"

GameEngine::GameEngine() : m_newBallCountOnMove(3)
{
    //ctor
}

GameEngine::~GameEngine()
{
    //dtor
}

void GameEngine::startNewGame(const int widthInTiles, const int heightInTiles, const int colorCount)
{
    m_tileMap.resize(heightInTiles, std::vector <Tile>(widthInTiles, Tile::Empty));
    for (auto& row : m_tileMap)
        std::fill(row.begin(), row.end(), Tile::Empty);

    m_selection = std::make_pair(-1, -1);
    m_state = GameState::FirstPick;

    m_isAdditionalMoveAvailable = false;
    m_timeElapsedInSeconds = 0;
    m_score = 0;

    m_colorCount = colorCount;
    m_minStreakLength = (widthInTiles < heightInTiles) ? (widthInTiles - 4) : (heightInTiles - 4);

    addExpectedBalls(m_newBallCountOnMove);
    transformExpectedBalls();
    addExpectedBalls(m_newBallCountOnMove);
}

/*
 * The entry point for making moves
 */
void GameEngine::processPick(const int row, const int column)
{
    // We always keep the game state in mind
    // to process different game situations differently
    // in a way easy to understand

    switch (m_state)
    {
        case GameState::FirstPick:
        {
            if (!isBall(m_tileMap[row][column]))
                break;

            selectTile(row, column);
            m_state = GameState::SecondPick;
            break;
        }

        case GameState::SecondPick:
        {
            // Deselect if player chooses the same ball
            if (m_selection == std::make_pair(row, column))
            {
                deselectTile();
                m_state = GameState::FirstPick;
            }

            // Process move if player selects a free cell
            else if (isTilePassable(m_tileMap[row][column]))
            {
                if (!pathExists(m_selection.first, m_selection.second, row, column))
                    break;

                swapSelectedWith(row, column);
                deselectTile();
                m_state = GameState::FirstPick;

                m_isAdditionalMoveAvailable = false;
                auto score = deleteStreaks(row, column);

                if (score > 0)
                {
                    increaseScore(score);
                    m_isAdditionalMoveAvailable = true;
                }

                if (!m_isAdditionalMoveAvailable)
                {
                    transformExpectedBalls();
                    auto ballsAdded = addExpectedBalls(m_newBallCountOnMove);

                    if (ballsAdded == 0)
                        m_state = GameState::GameOver;
                }
            }

            // Select another ball if player selects it
            else
            {
                deselectTile();
                selectTile(row, column);
            }

            break;
        }

        default:
            break;
    }
}

void GameEngine::selectTile(const int row, const int column)
{
    m_tileMap[row][column] = normalToSelected(m_tileMap[row][column]);
    m_selection = std::make_pair(row, column);
}

void GameEngine::deselectTile()
{
    auto row = m_selection.first;
    auto column = m_selection.second;

    m_tileMap[row][column] = selectedToNormal(m_tileMap[row][column]);
    m_selection = std::make_pair(-1, -1);
}

void GameEngine::swapSelectedWith(const int rowNew, const int columnNew)
{
    auto rowOld = m_selection.first;
    auto columnOld = m_selection.second;

    std::swap(m_tileMap[rowNew][columnNew], m_tileMap[rowOld][columnOld]);
    m_selection = std::make_pair(rowNew, columnNew);
}

/*
 * The count of free cells can be less than the required number of balls
 * So, it adds balls as maximum as possible
 * Returns the number of added balls
 */
int GameEngine::addExpectedBalls(const int maxCount)
{
    /* We create a vector of coordinates of empty cells
     * to randomly choose a pair of coordinates
     * to place a ball in the randomly selected cell
     */

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

    const int countAdded = (maxCount > static_cast <int>(emptyTiles.size())) ? static_cast <int>(emptyTiles.size()) : maxCount;

    for (auto i = 0; i < countAdded; )
    {
        auto index = m_random.getInteger(0, emptyTiles.size());

        auto row = emptyTiles[index].first;
        auto column = emptyTiles[index].second;

        // We do not delete cells that we have filled, so there's a workaround
        if (m_tileMap[row][column] != Tile::Empty)
            continue;

        m_tileMap[row][column] = m_random.getTile(Tile::ExpectedColorOne,
                                                  Tile::ExpectedColorOne + static_cast <Tile>(m_colorCount));
        i++;
    }

    return countAdded;
}

/*
 * Transforms balls to real ones
 * deletes groups if they appear
 * and gives scores (but not an additional move)
 */
void GameEngine::transformExpectedBalls()
{
    for (auto row = 0; row < m_tileMap.size(); row++)
    {
        for (auto column = 0; column < m_tileMap.size(); column++)
        {
            if (isExpected(m_tileMap[row][column]))
            {
                m_tileMap[row][column] = expectedToNormal(m_tileMap[row][column]);

                auto score = deleteStreaks(row, column);
                increaseScore(score);
            }
        }
    }
}

bool GameEngine::isTilePassable(const Tile t) const
{
    return (t == Tile::Empty || isExpected(t));
}

/*
 * Uses BFS to find if a path between to cells exists
 * Does not count diagonal moves, only horizontal and vertical
 */
bool GameEngine::pathExists(const int sourceRow,
                            const int sourceColumn,
                            const int destinationRow,
                            const int destinationColumn) const
{
    const std::vector <std::pair <int, int>> offsets {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

    std::vector <std::pair <int, int>> visited;
    visited.reserve(m_tileMap.size() * m_tileMap[0].size());

    std::queue <std::pair <int, int>> q;
    q.push(std::make_pair(sourceRow, sourceColumn));

    while (q.size() > 0)
    {
        auto p = q.front();
        q.pop();

        visited.push_back(p);

        if (p == std::make_pair(destinationRow, destinationColumn))
            return true;

        for (auto i = 0; i < offsets.size(); i++)
        {
            const auto nextRow    = p.first  + offsets[i].first;
            const auto nextColumn = p.second + offsets[i].second;
            const auto nextTile   = std::make_pair(nextRow, nextColumn);

            if (nextRow    >= 0 && nextRow    < m_tileMap.size()    &&
                nextColumn >= 0 && nextColumn < m_tileMap[0].size() &&
                isTilePassable(m_tileMap[nextRow][nextColumn]) &&
                std::find(visited.begin(), visited.end(), nextTile) == visited.end())
            {
                q.push(nextTile);
            }
        }
    }

    return false;
}

/*
 * Finds all possible streaks for a ball and deletes them
 * Returns earned amount of points
 */
int GameEngine::deleteStreaks(const int row, const int column)
{
    // We check all possible directions,
    // then we delete only adjacent balls if combinations are found.
    // We delete the selected ball only after we delete balls in all directions
    // so that we do not lose a situation when the ball makes several lines

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
        return totalStreakLength;

    m_tileMap[row][column] = Tile::Empty;
    totalStreakLength++;

    return totalStreakLength;
}

void GameEngine::increaseScore(const int streakLength)
{
    // The more length is, the more points for each ball are given
    m_score += streakLength * (streakLength - m_minStreakLength + 1);
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

bool GameEngine::isGameOver() const
{
    return (m_state == GameState::GameOver);
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

int GameEngine::getColorCount() const
{
    return m_colorCount;
}
