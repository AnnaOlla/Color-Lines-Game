#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "GameEngine.hpp"

#include <string>
#include <stdexcept>
#include <fstream>
#include <iomanip>

class Logger
{
    public:
        Logger();
        virtual ~Logger();

        void writeError(const GameEngine&, const std::exception&);
};

#endif // LOGGER_HPP
