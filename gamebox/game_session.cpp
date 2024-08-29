#include "game_session.h"
#include "game.h"
#include "user.h"

int GameSession::count = 0;
locker GameSession::c_lock;

GameSession::GameSession(Game* game)
    :gameInstance(game), status(SESSION_STATUS::WAIT), numUsers(1)
    {
        printf("gameInstance is  %s\n", gameInstance->getName().c_str());
        sessionId = GameSession::getCount();
        gameInstance->init();
        maxUsers = gameInstance->getPlayerLimit();
        printf("GameSession create end\n");
    }


bool GameSession::startGame()
{
    gameInstance->play();
}


bool GameSession::endGame()
{
    gameInstance->end();
}


bool GameSession::addUser(std::shared_ptr<User> u)
{
    if(numUsers < maxUsers)
    {
        s_lock.lock();
        userList.push_back(u);
        status = numUsers == maxUsers?SESSION_STATUS::FULL : SESSION_STATUS::WAIT;
        s_lock.unlock();
        return true;
    }
    return false;
}


bool GameSession::rmUser(std::shared_ptr<User> u)
{
    if(numUsers > 0)
    {
        s_lock.lock();
        userList.remove_if([u](std::shared_ptr<User>& uu) {
            return uu->getName() == u->getName(); 
        });
        status = SESSION_STATUS::WAIT;
        s_lock.unlock();
        return true;
    }
    return false;
}


const std::list<std::shared_ptr<User>>& GameSession::displayUsers() const
{
    return userList;
}


const int GameSession::getSessionId() const
{
    return sessionId;
}


GAMETYPE GameSession::getGameType() const
{
    return gameInstance->getType();
}



const std::string& GameSession::getGameName() const
{
    return gameInstance->getName();
}



const int GameSession::getUserLimit() const
{
    return maxUsers;
}


const int GameSession::getUserNum() const
{
    return numUsers;
}



int GameSession::getCount()
{
    GameSession::c_lock.lock();
    GameSession::count++;
    GameSession::c_lock.unlock();
    return GameSession::count;
}


json GameSession::toJson() const
{
    return json{
        {"sessionId", sessionId},
        {"gameType", getGameType()},
        {"maxUsers", maxUsers},
        {"numUsers", numUsers},
        {"status", status},
        };
}

json GameSession::getSessionInfo() const{
    json sj = toJson();
    json uj = ptrListToJson(userList);
    sj["users"] = uj;
    sj["success"] = true;
    return sj;
}

std::string GameSession::excute()
{
    gameInstance->excute();
}

const std::list<std::shared_ptr<User>>& GameSession::getUserList()
{
    return userList;
}