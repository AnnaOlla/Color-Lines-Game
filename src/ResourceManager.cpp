#include "ResourceManager.hpp"

ResourceManager::ResourceManager() : m_spriteSizeInPixels(64), m_directoryName("resources/")
{
    //ctor
}

ResourceManager::~ResourceManager()
{
    //dtor
}

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

/*
 * This internal function makes a scaled sprite
 * The difference between setScale is that the scale of the sprite stays the same,
 * but its texture rectangle gets resized
*/
void ResourceManager::loadScaledSprite(const sf::Texture& texture, sf::Sprite& sprite, float factor)
{
    /*
     * Let 'x' be the original width of texture and 'a' the resize factor
     * Then ax will be resized width
     * We place the new texture at the center of the old one
     * So, the new texture has width:
     * (ax - x) / 2 to the left, (ax - x) / 2 to the right of old texture and x in the center
     * Top left corner of rectangle is placed in -(ax - x), the width is ax
     *
     * Small trick:
     * 1. Texture rectangle is enlarged to make sprite smaller
     * 2. Texture rectangle is shrinked to make sprite bigger
     * So, the factor should be reversed to make external usage intuitive
     *
     * Same math for the height
    */

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

sf::Sprite ResourceManager::getBallSprite(const Tile tile) const
{
    return m_ballSprites.at(tile);
}

sf::Sprite ResourceManager::getCellSprite() const
{
    return m_cellSprite;
}

int ResourceManager::getSpriteSize() const
{
    return m_spriteSizeInPixels;
}

sf::Font ResourceManager::getFont() const
{
    return m_font;
}
