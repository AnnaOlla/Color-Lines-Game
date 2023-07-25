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
        ResourceManager();
        ~ResourceManager();

        void loadFont();
        void loadSprites();

        sf::Sprite getBallSprite(const Tile) const;
        sf::Sprite getCellSprite() const;
        int getSpriteSize() const;
        sf::Font getFont() const;

    private:
        void loadSprite(sf::Texture&, sf::Sprite&, const std::string&);
        void loadScaledSprite(const sf::Texture&, sf::Sprite&, const float);

        sf::Font m_font;
        sf::Texture m_cellTexture;
        sf::Sprite m_cellSprite;

        std::map <Tile, sf::Texture> m_ballTextures;
        std::map <Tile, sf::Sprite> m_ballSprites;

        const int m_spriteSizeInPixels;
        const std::string m_directoryName;
};

#endif // RESOURCEMANAGER_HPP
