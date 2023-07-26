#include "ResourceManager.hpp"
#include "GameEngine.hpp"
#include "UserInterface.hpp"
#include "Logger.hpp"

int main()
{
    Logger logger;
    ResourceManager resourceManager;
    GameEngine game;

    try
    {
        resourceManager.loadSprites();
        resourceManager.loadFont();

        game.startNewGame(9, 9, 8);

        UserInterface ui(game, resourceManager);
        ui.startMainLoop();
    }
    catch (const std::exception& e)
    {
        logger.writeError(game, e);
    }

    return 0;
}
