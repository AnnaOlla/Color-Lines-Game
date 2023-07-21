#include "ResourceManager.hpp"
#include "GameEngine.hpp"

#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    ResourceManager::loadSprites();

    auto tileMapSize = 9;
    auto colorCount = 8;

    const auto infoPanelHeight = 128;
    auto spriteSize = ResourceManager::getSpriteSize();

    auto windowWidth = tileMapSize * spriteSize;
    auto windowHeight = tileMapSize * spriteSize + infoPanelHeight;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Lines");
    window.setFramerateLimit(30);
    window.setVerticalSyncEnabled(true);

    GameEngine game;
    game.startNewGame(tileMapSize, colorCount);

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::MouseButtonPressed)
            {
                auto position = sf::Mouse::getPosition(window);

                if (position.y > infoPanelHeight)
                {
                    const auto m = game.getTileMap();

                    for (auto& i : m)
                    {
                        for (auto& j : i)
                            std::cout << static_cast <int>(j) << ' ';
                        std::cout << '\n';
                    }

                    auto row = (position.y - infoPanelHeight) / spriteSize;
                    auto column = position.x / spriteSize;
                    game.processMove(row, column);
                }
            }

            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        const auto tileMap = game.getTileMap();
        for (size_t i = 0; i < tileMap.size(); i++)
        {
            for (size_t j = 0; j < tileMap[i].size(); j++)
            {
                sf::Vector2f position(j * spriteSize, i * spriteSize + infoPanelHeight);

                auto cellSprite = ResourceManager::getCellSprite();
                cellSprite.setPosition(position);
                window.draw(cellSprite);

                if (tileMap[i][j] == Tile::Empty)
                    continue;

                auto ballSprite = ResourceManager::getBallSprite(tileMap[i][j]);
                ballSprite.setPosition(position);
                window.draw(ballSprite);
            }
        }

        window.display();
    }

    return 0;
}
