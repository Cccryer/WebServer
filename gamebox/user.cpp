#include "user.h"
#include "game_session.h"
#include "game.h"


const int User::getScore(std::string gname)
{
    return scoresMap[gname];
}


void User::setScore(std::string gname, int sc)
{
    scoresMap[gname] = sc;
}


std::shared_ptr<GameSession> User::getSession() const
{
    return currentSession;
}


void User::setSession(std::shared_ptr<GameSession> session)
{
    currentSession = session;
}


const GAMETYPE User::getCurGameType()
{
    return currentGameType;
}


void User::setCurGameType(GAMETYPE t)
{
    currentGameType = t;
}


json User:: toJson() const
{
    int sc;
    if(currentSession != nullptr)
    {
        std::string gname = currentSession->getGameName();
        auto it = scoresMap.find(gname);
        sc = it != scoresMap.end()?it->second:0;
    }
    return json{
        {"name", name},
        {"id", id},
        {"score", sc},
    };    
}

conn* User::getConn()
{
    return connptr;
}