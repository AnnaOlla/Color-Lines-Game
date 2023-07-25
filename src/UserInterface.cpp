#include "UserInterface.hpp"

UserInterface::UserInterface(GameEngine& game, const ResourceManager& resourceManager) :
    m_game(game),
    m_resourceManager(resourceManager),
    m_window(sf::VideoMode(resourceManager.getSpriteSize() * game.getTileMapWidth(),
                           resourceManager.getSpriteSize() * (game.getTileMapHeight() + 1)),
             "Lines",
             sf::Style::Close),
    m_infoPanel(sf::Vector2f(m_resourceManager.getSpriteSize() * game.getTileMapWidth(),
                             m_resourceManager.getSpriteSize())),
    m_textColor(0x35, 0xC5, 0xFF),
    m_font(m_resourceManager.getFont())
{
    m_window.setFramerateLimit(30);
    m_window.setVerticalSyncEnabled(true);

    m_infoPanel.setFillColor(sf::Color::Black);

    m_scoreText.setFillColor(m_textColor);
    m_scoreText.setCharacterSize(m_infoPanel.getSize().y / 2);
    m_scoreText.setFont(m_font);

    m_timeText.setFillColor(m_textColor);
    m_timeText.setCharacterSize(m_infoPanel.getSize().y / 2);
    m_timeText.setFont(m_font);

    m_gameOverPanel.setFillColor(sf::Color(0, 0, 0, 192));

    m_gameOverText.setFillColor(m_textColor);
    m_gameOverText.setCharacterSize(m_infoPanel.getSize().y / 2);
    m_gameOverText.setFont(m_font);
    m_gameOverText.setString("GAME OVER");
}

UserInterface::~UserInterface()
{
    //dtor
}

void UserInterface::startMainLoop()
{
    const auto spriteSize = m_resourceManager.getSpriteSize();
    auto elapsedSeconds = 0.0f;

    while (m_window.isOpen())
    {
        elapsedSeconds += m_clock.restart().asSeconds();
        if (elapsedSeconds >= m_maxClockDelayInSeconds && !m_game.isGameOver())
        {
            m_game.increaseTimer();
            elapsedSeconds = 0.0f;
        }

        sf::Event event;

        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::MouseButtonPressed)
            {
                auto position = sf::Mouse::getPosition(m_window);

                if (!m_game.isGameOver() && position.y >= m_infoPanel.getLocalBounds().height + m_infoPanel.getLocalBounds().top)
                {
                    auto row = (position.y - m_infoPanel.getSize().y) / spriteSize;
                    auto column = position.x / spriteSize;

                    m_game.processPick(row, column);
                }
                else
                {
                    m_game.startNewGame(m_game.getTileMapWidth(), m_game.getTileMapHeight(), m_game.getColorCount());
                }
            }

            if (event.type == sf::Event::Closed)
                m_window.close();
        }

        renderGame();
    }
}

void UserInterface::renderGame()
{
    m_window.clear();

    renderInfoPanel();
    renderTileMap();

    if (m_game.isGameOver())
        renderGameOverPanel();

    m_window.display();
}

void UserInterface::renderInfoPanel()
{
    m_window.draw(m_infoPanel);

    const float xMarginInPixels = 10;

    std::ostringstream oss;

    const auto score = m_game.getScore();
    oss << std::setfill('0') << std::setw(7) << score;
    m_scoreText.setString(oss.str());

    auto x = m_infoPanel.getSize().x - m_scoreText.getLocalBounds().width - m_scoreText.getLocalBounds().left - xMarginInPixels;
    auto y = (m_infoPanel.getSize().y - m_scoreText.getLocalBounds().height - m_scoreText.getLocalBounds().top) / 2;
    m_scoreText.setPosition(x, y);
    m_window.draw(m_scoreText);

    oss.str(std::string());

    const auto seconds = m_game.getTimeInSeconds();
    oss << seconds / (60 * 60) << ':'
        << std::setfill('0') << std::setw(2) << (seconds / 60) % 60 << ':'
        << std::setfill('0') << std::setw(2) << seconds % 60;
    m_timeText.setString(oss.str());

    x = xMarginInPixels;
    y = (m_infoPanel.getSize().y - m_timeText.getLocalBounds().height - m_timeText.getLocalBounds().top) / 2;
    m_timeText.setPosition(x, y);
    m_window.draw(m_timeText);
}

void UserInterface::renderTileMap()
{
    const auto tileMap = m_game.getTileMap();
    const auto spriteSize = m_resourceManager.getSpriteSize();

    auto cellSprite = m_resourceManager.getCellSprite();

    for (size_t i = 0; i < tileMap.size(); i++)
    {
        for (size_t j = 0; j < tileMap[i].size(); j++)
        {
            sf::Vector2f position(j * spriteSize, i * spriteSize + m_infoPanel.getSize().y);

            cellSprite.setPosition(position);
            m_window.draw(cellSprite);

            if (tileMap[i][j] == Tile::Empty)
                continue;

            auto ballSprite = m_resourceManager.getBallSprite(tileMap[i][j]);
            ballSprite.setPosition(position);
            m_window.draw(ballSprite);
        }
    }
}

void UserInterface::renderGameOverPanel()
{
    const auto left = m_infoPanel.getLocalBounds().left;
    const auto width = m_infoPanel.getLocalBounds().width;

    const auto top = m_infoPanel.getLocalBounds().top + m_infoPanel.getLocalBounds().height;
    const auto height = m_window.getSize().y - top;

    m_gameOverPanel.setPosition(left, top);
    m_gameOverPanel.setSize(sf::Vector2f(width, height));
    m_window.draw(m_gameOverPanel);

    const auto x = (width + left - m_gameOverText.getLocalBounds().left - m_gameOverText.getLocalBounds().width) / 2;
    const auto y = (height + top - m_gameOverText.getLocalBounds().top - m_gameOverText.getLocalBounds().height) / 2;

    m_gameOverText.setPosition(x, y);
    m_window.draw(m_gameOverText);
}
