#include "ResourceManager.hpp"
#include "GameEngine.hpp"
#include "UserInterface.hpp"

int main()
{
    ResourceManager::loadSprites();
    ResourceManager::loadFont();

    GameEngine game;
    game.startNewGame(9, 9, 8);

    UserInterface ui(game);
    ui.mainLoop();

    return 0;
}
