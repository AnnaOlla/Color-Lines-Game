#include "ResourceManager.hpp"

ResourceManager::ResourceManager()
{
    //ctor
}

ResourceManager::~ResourceManager()
{
    //dtor
}

sf::Font ResourceManager::m_font;
sf::Texture ResourceManager::m_cellTexture;
sf::Sprite ResourceManager::m_cellSprite;

std::map <Tile, sf::Texture> ResourceManager::m_ballTextures;
std::map <Tile, sf::Sprite> ResourceManager::m_ballSprites;

const int ResourceManager::m_spriteSizeInPixels = 64;
const std::string ResourceManager::m_directoryName = "resources/";

void ResourceManager::loadFont()
{
    auto relativePath = m_directoryName + "DigitalNumbers-Regular.ttf";

    auto isLoadSuccessful = m_font.loadFromFile(relativePath);
    if (!isLoadSuccessful)
        throw std::runtime_error("Cannot load file " + relativePath);
}

void ResourceManager::loadSprite(sf::Texture& texture, sf::Sprite& sprite, const std::string& relativePath)
{
    auto isLoadSuccessful = texture.loadFromFile(relativePath);

    if (!isLoadSuccessful)
        throw std::runtime_error("Cannot load file " + relativePath);

    texture.setSmooth(true);
    sprite.setTexture(texture);
    sprite.setScale(m_spriteSizeInPixels / sprite.getLocalBounds().width,
                    m_spriteSizeInPixels / sprite.getLocalBounds().height);
}

void ResourceManager::loadScaledSprite(const sf::Texture& texture, sf::Sprite& sprite, float factor)
{
    factor = 1 / factor;

    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(sprite.getLocalBounds().width  * (1.0f - factor) / 2,
                                      sprite.getLocalBounds().height * (1.0f - factor) / 2,
                                      sprite.getLocalBounds().width  * factor,
                                      sprite.getLocalBounds().height * factor));

    sprite.setScale(m_spriteSizeInPixels / sprite.getLocalBounds().width,
                    m_spriteSizeInPixels / sprite.getLocalBounds().height);
}

void ResourceManager::loadSprites()
{
    loadSprite(m_cellTexture, m_cellSprite, m_directoryName + "cell.png");

    for (auto tile = Tile::ColorOne; tile < Tile::ColorEnd; tile++)
    {
        auto fileName = m_directoryName + "ball_" + std::to_string(static_cast <int>(tile)) + ".png";
        loadSprite(m_ballTextures[tile], m_ballSprites[tile], fileName);

        loadScaledSprite(m_ballTextures[tile], m_ballSprites[normalToExpected(tile)], 0.5f);
        loadScaledSprite(m_ballTextures[tile], m_ballSprites[normalToSelected(tile)], 1.5f);
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

int ResourceManager::getSpriteSize()
{
    return m_spriteSizeInPixels;
}

sf::Font ResourceManager::getFont()
{
    return m_font;
}
