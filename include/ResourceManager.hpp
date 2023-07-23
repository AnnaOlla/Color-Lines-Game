#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include "Tile.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Font.hpp>

#include <string>
#include <map>

class ResourceManager
{
    public:
        static void loadFont();
        static void loadSprites();

        static sf::Sprite getBallSprite(const Tile);
        static sf::Sprite getCellSprite();
        static int getSpriteSize();
        static sf::Font getFont();

    private:
        ResourceManager();
        ~ResourceManager();

        static void loadSprite(sf::Texture&, sf::Sprite&, const std::string&);
        static void loadScaledSprite(const sf::Texture&, sf::Sprite&, const float);

        static sf::Font m_font;
        static sf::Texture m_cellTexture;
        static sf::Sprite m_cellSprite;

        static std::map <Tile, sf::Texture> m_ballTextures;
        static std::map <Tile, sf::Sprite> m_ballSprites;

        static const std::string m_directoryName;
        static const int m_spriteSizeInPixels;
};

#endif // RESOURCEMANAGER_HPP
