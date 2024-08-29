#include "game_manager.h"
#include "game_session.h"
#include "game.h"
#include "gomoku.h"
#include "user.h"
#include "../lock/locker.h"


Game* GameFactory::createGame(GAMETYPE type)
{
    switch(type){
        case GAMETYPE::GOMOKU:
        {
            printf("Factory hint GOMOKU\n");
            GomokuGame *gp =  new GomokuGame();
            return gp;
        }
        case GAMETYPE::SNAKE:
        default:
            return nullptr;
        
    }
}



bool GameManager::addGameSession(std::shared_ptr<GameSession> session)
{
    if(session == nullptr)
        return false;
    m_lock.wrlock();
    gameSenssions[session->getGameType()].push_back(session);
    std::string gname = session->getGameName();
    std::string sid = std::to_string(session->getSessionId());
    printf("insert key: %s, value: %d\n", (gname+sid).c_str(),  session->getSessionId());
    idToSessionsMap[gname + sid] = session;
    m_lock.unlock();
    return true;
}


bool GameManager::rmGamesession(std::shared_ptr<GameSession> session)
{
    if(session == nullptr)
        return false;
    m_lock.wrlock();
    gameSenssions[session->getGameType()]
        .remove_if([session](std::shared_ptr<GameSession> s){return session->getSessionId() == s->getSessionId();});
    std::string gname = session->getGameName();
    std::string sid = std::to_string(session->getSessionId());
    idToSessionsMap[gname + sid] = nullptr;
    m_lock.unlock();
    return true;
}


bool GameManager::addUser(std::shared_ptr<User> u)
{
    if(u == nullptr)
        return false;
    lst_lock.wrlock();
    users.push_back(u);
    lst_lock.unlock();
    return true;
}


bool GameManager::rmUser(std::shared_ptr<User> u)
{
    if(u == nullptr)
        return false;
    lst_lock.wrlock();
    users.remove_if([u](std::shared_ptr<User> uu){ return uu->getName() == u->getName();});
    lst_lock.unlock();
    return true;
}


std::shared_ptr<GameSession> GameManager::createGameSession(std::shared_ptr<User> u, GAMETYPE type)
{
    printf("create gamesession\n");
    Game *upg = GameFactory::createGame(type);
    if(upg != nullptr)
    {
        // GameSession* gs = new GameSession(upg);
        // std::shared_ptr<GameSession> s(gs);
        auto gs = std::make_shared<GameSession>(upg);
        if(gs)
        {
            gs->addUser(u);
            u->setCurGameType(type);
            u->setSession(gs);
            GameManager::get_manager().addGameSession(gs);
            return gs;
        }
    }
    return nullptr;
}


bool GameManager::joinGameSession(std::shared_ptr<User> u, std::string gname, std::string sid)
{
    printf("userName is: %s, gname is: %s, sid is : %s\n", u->getName().c_str(), gname.c_str(), sid.c_str());
    if(u == nullptr)
        return false;
    m_lock.rdlock();
    printf("query key is: %s\n", (gname+sid).c_str());
    auto iter = idToSessionsMap.find(gname + sid);

    if(iter != idToSessionsMap.end())
    {

        auto session = iter->second;
        m_lock.unlock();

        if(session->addUser(u))
        {
            u->setSession(session);
            u->setCurGameType(session->getGameType());
            return true;
        }

        return false;
    }
    else
    {
        m_lock.unlock();
        return false;
    }
}


bool GameManager::quitGameSession(std::shared_ptr<User> u, std::string gname, std::string gid)
{
    if(u == nullptr)
        return false;
    m_lock.rdlock();
    auto iter = idToSessionsMap.find(gname + gid);
    
    if(iter != idToSessionsMap.end())
    {
        auto session = iter->second;
        m_lock.unlock();

        u->setSession(nullptr);
        session->rmUser(u);
        if(session->getUserNum() == 0)
        rmGamesession(session);
        return true;
    }
    else
    {
        m_lock.unlock();
        return false;
    }
}


const std::list<std::shared_ptr<GameSession>>
GameManager::getGameSessions(GAMETYPE type)
{
    return gameSenssions[type];
}


std::shared_ptr<User> GameManager::createUser(std::string name, int id)
{
    auto u = std::make_shared<User>(name, id);
    GameManager::get_manager().addUser(u);
    return u;
}


bool GameManager::offlineUser(std::shared_ptr<User> u)
{
    u->setStatus(false);
    rmUser(u);
}

