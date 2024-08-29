#include "game.h"


const std::string& Game::getName() const
{
    return gameName;
}


const std::string& Game::getRule() const
{
    return gameRule;
}


const GAMETYPE Game::getType() const
{
    return gameType;
}


const int Game::getPlayerLimit() const
{
    return maxPlayers;
}