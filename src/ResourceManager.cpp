#include "ResourceManager.hpp"

ResourceManager::ResourceManager()
{
    //ctor
}

ResourceManager::~ResourceManager()
{
    //dtor
}

sf::Texture ResourceManager::m_cellTexture;
sf::Sprite ResourceManager::m_cellSprite;
std::map <Tile, sf::Texture> ResourceManager::m_ballTextures;
std::map <Tile, sf::Sprite> ResourceManager::m_ballSprites;

void ResourceManager::loadSprite(sf::Texture& texture, sf::Sprite& sprite, const std::string& relativePath)
{
    auto isLoadSuccessful = texture.loadFromFile(relativePath);

    if (!isLoadSuccessful)
        throw std::runtime_error(std::string("Cannot load file ") + relativePath);

    sprite.setTexture(texture);
}

void ResourceManager::loadSprites()
{
    const std::string directoryName("resources/");
    loadSprite(m_cellTexture, m_cellSprite, directoryName + "cell.png");

    for (auto tile = Tile::ColorOne; tile < Tile::ColorEnd; tile++)
    {
        auto fileName = directoryName + "ball_" + std::to_string(static_cast <int>(tile)) + ".png";
        loadSprite(m_ballTextures[tile], m_ballSprites[tile], fileName);
    }

    for (auto tile = Tile::ExpectedColorOne; tile < Tile::ExpectedColorEnd; tile++)
    {
        auto fileName = directoryName + "expected_ball_" + std::to_string(static_cast <int>(tile)) + ".png";
        loadSprite(m_ballTextures[tile], m_ballSprites[tile], fileName);
    }

    for (auto tile = Tile::SelectedColorOne; tile < Tile::SelectedColorEnd; tile++)
    {
        auto fileName = directoryName + "selected_ball_" + std::to_string(static_cast <int>(tile)) + ".png";
        loadSprite(m_ballTextures[tile], m_ballSprites[tile], fileName);
    }
}

sf::Sprite ResourceManager::getBallSprite(const Tile tile)
{
    return m_ballSprites.at(tile);
}

sf::Sprite ResourceManager::getCellSprite()
{
    return m_cellSprite;
}
