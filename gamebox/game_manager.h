#ifndef GAME_ROOM_MANAGER
#define GAME_ROOM_MANAGER

#include<unordered_map>
#include<list>
#include "jsoner.h"
#include "../lock/locker.h"
// enum class GAMETYPE{
//     GOMOKU,
//     SNAKE
// };

// constexpr GAMETYPE allGameTypes[] = {
//     GAMETYPE::GOMOKU,
//     GAMETYPE::SNAKE
//     // 继续添加其他游戏类型
// };

const std::string gamelist[] = {"gomoku", "snake"};

enum class GAMETYPE;
class GameSession;
class User;
class Game;

class GameFactory
{
    public:
        static Game* createGame(GAMETYPE type);
    private:
        GameFactory() = delete;
};

struct EnumClassHash
{
    template <typename T>
    std::size_t operator()(T t) const
    {
        return static_cast<std::size_t>(t);
    }
};

class GameManager
{
    public:
    static GameManager& get_manager(){
        static GameManager manager;
        return manager;
    }
    bool addGameSession(std::shared_ptr<GameSession>);
    bool rmGamesession(std::shared_ptr<GameSession>);
    bool addUser(std::shared_ptr<User>);
    bool rmUser(std::shared_ptr<User>);

    std::shared_ptr<User> createUser(std::string, int);
    bool offlineUser(std::shared_ptr<User>);

    std::shared_ptr<GameSession> createGameSession(std::shared_ptr<User>, GAMETYPE);
    bool joinGameSession(std::shared_ptr<User>, std::string, std::string);
    bool quitGameSession(std::shared_ptr<User>, std::string, std::string);

    const std::list<std::shared_ptr<GameSession>> getGameSessions(GAMETYPE);

    json displayGames() { return  json(gamelist);}

    private:
        GameManager() = default;
        std::unordered_map<GAMETYPE, std::list<std::shared_ptr<GameSession>>, EnumClassHash> gameSenssions;
        std::unordered_map<std::string, std::shared_ptr<GameSession>> idToSessionsMap; //gameid+sessionid -> session
        std::list<std::shared_ptr<User>> users;
        rwlocker m_lock;
        rwlocker lst_lock;
};

#endif