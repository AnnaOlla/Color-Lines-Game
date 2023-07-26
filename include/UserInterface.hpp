#ifndef USERINTERFACE_HPP
#define USERINTERFACE_HPP

#include "ResourceManager.hpp"
#include "GameEngine.hpp"

#include <SFML/Graphics.hpp>

#include <sstream>
#include <iomanip>

class UserInterface
{
    public:
        UserInterface(GameEngine&, const ResourceManager&);
        virtual ~UserInterface();

        void startMainLoop();
        void renderGame();

    private:
        GameEngine& m_game;
        const ResourceManager& m_resourceManager;

        sf::RenderWindow m_window;

        sf::Clock m_clock;
        float m_elapsedSeconds;
        const float m_maxClockDelayInSeconds;

        sf::RectangleShape m_infoPanel;
        sf::Color m_textColor;
        sf::Font m_font;
        sf::Text m_scoreText;
        sf::Text m_timeText;
        sf::RectangleShape m_gameOverPanel;
        sf::Text m_gameOverText;

        void processTimer();
        void processClick();

        void renderInfoPanel();
        void renderTileMap();
        void renderGameOverPanel();
};

#endif // USERINTERFACE_HPP
