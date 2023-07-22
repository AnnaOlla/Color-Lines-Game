#ifndef USERINTERFACE_HPP
#define USERINTERFACE_HPP

#include "ResourceManager.hpp"
#include "GameEngine.hpp"

#include <SFML/Graphics.hpp>

class UserInterface
{
    public:
        UserInterface(GameEngine&);
        virtual ~UserInterface();

        void mainLoop();
        void processClick();
        void renderGame();

    private:
        GameEngine& m_game;

        sf::RenderWindow m_window;

        sf::Clock m_clock;
        const float m_maxClockDelayInSeconds = 1.0f;

        sf::RectangleShape m_infoPanel;
        sf::Color m_textColor;
        sf::Font m_font;
        sf::Text m_scoreText;
        sf::Text m_timeText;

        void renderTileMap();
        void renderInfoPanel();
};

#endif // USERINTERFACE_HPP
