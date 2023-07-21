#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include "Tile.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <string>
#include <map>

class ResourceManager
{
    public:
        static void loadSprites();
        static sf::Sprite getBallSprite(const Tile);
        static sf::Sprite getCellSprite();

    private:
        ResourceManager();
        ~ResourceManager();

        static void loadSprite(sf::Texture&, sf::Sprite&, const std::string&);

        static sf::Texture m_cellTexture;
        static sf::Sprite m_cellSprite;

        static std::map <Tile, sf::Texture> m_ballTextures;
        static std::map <Tile, sf::Sprite> m_ballSprites;
};

#endif // RESOURCEMANAGER_HPP
