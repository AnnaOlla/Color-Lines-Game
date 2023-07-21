#include "RandomNumberGenerator.hpp"

RandomNumberGenerator::RandomNumberGenerator() : m_engine(std::chrono::system_clock::now().time_since_epoch().count())
{
    //ctor
}

RandomNumberGenerator::~RandomNumberGenerator()
{
    //dtor
}

int RandomNumberGenerator::getInteger(const int inclusiveMinValue, const int exclusiveMaxValue)
{
    std::uniform_int_distribution <int> distribution(inclusiveMinValue, exclusiveMaxValue - 1);
    return distribution(m_engine);
}

Tile RandomNumberGenerator::getTile(const Tile inclusiveMinValue, const Tile exclusiveMaxValue)
{
    auto min = static_cast <int>(inclusiveMinValue);
    auto max = static_cast <int>(exclusiveMaxValue);
    return static_cast <Tile>(getInteger(min, max));
}
