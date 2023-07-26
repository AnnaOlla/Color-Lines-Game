#include "Logger.hpp"

Logger::Logger()
{
    //ctor
}

Logger::~Logger()
{
    //dtor
}

void Logger::writeError(const GameEngine& game, const std::exception& exception)
{
    std::ofstream logFile("error.log");
    if (!logFile)
        return;

    logFile << "An error occured!\n\n"
            << "Reason: "     << exception.what()        << '\n'
            << "Game state: " << game.getState()         << '\n'
            << "Game time: "  << game.getTimeInSeconds() << '\n'
            << "Game score: " << game.getScore()         << '\n'
            << "Game map:\n";

    const auto& map = game.getTileMap();
    for (const auto& row : map)
    {
        for (const auto& tile : row)
            logFile << std::setfill(' ') << std::setw(2) << std::right << static_cast <int>(tile) << ' ';
        logFile << '\n';
    }
}
