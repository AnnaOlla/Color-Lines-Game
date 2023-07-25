#include "ResourceManager.hpp"
#include "GameEngine.hpp"
#include "UserInterface.hpp"

int main()
{
    ResourceManager resourceManager;
    resourceManager.loadSprites();
    resourceManager.loadFont();

    GameEngine game;
    game.startNewGame(9, 9, 8);

    UserInterface ui(game, resourceManager);
    ui.startMainLoop();

    return 0;
}
