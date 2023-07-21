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

    m_tileMap[rowNew][columnNew] = m_tileMap[rowOld][columnOld] - Tile::SelectedColorStart;
    deselectTile();

    return true;
}

void GameEngine::addExpectedBalls(const int maxCount)
{
    /*
    We will check all tiles in order to:
    1. have count of them
    2. have their coordinates
    This way is easier than random tile check and safer if empty tiles count is less than required
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

    const int count = (maxCount >= static_cast <int>(emptyTiles.size())) ? maxCount : static_cast <int>(emptyTiles.size());

    for (auto i = 0; i < count; )
    {
        auto index = random.getInteger(0, emptyTiles.size());

        auto row = emptyTiles[index].first;
        auto column = emptyTiles[index].second;

        // We do not remove pairs that we have checked (it's not effective), so there is a workaround
        if (m_tileMap[row][column] == Tile::Empty)
            continue;

        m_tileMap[row][column] = random.getTile(Tile::ExpectedColorOne, Tile::ExpectedColorEnd);
        i++;
    }
}

void GameEngine::transformExpectedBalls()
{
    for (auto& row : m_tileMap)
    {
        for (auto& tile : row)
        {
            if (tile > Tile::ExpectedColorStart && tile < Tile::ExpectedColorEnd)
            {
                tile = tile - Tile::ExpectedColorStart;
                // TODO: delete possible combination
            }
        }
    }
}
