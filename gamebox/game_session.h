#ifndef GAME_SESSION_H
#define GAME_SESSION_H
#include<list>
#include<memory>
#include "../lock/locker.h"
#include "jsoner.h"

class User;
class Game;
enum class GAMETYPE;

enum class SESSION_STATUS
{
    WAIT,
    FULL,
    GAMING
};
class GameSession:public JsonSerializable
{
    public:
        GameSession(Game*);
        ~GameSession() = default;

        bool startGame();
        bool endGame();

        bool addUser(std::shared_ptr<User>);
        bool rmUser(std::shared_ptr<User>);
        const std::list<std::shared_ptr<User>>& displayUsers() const;

        const int getSessionId() const;

        GAMETYPE getGameType() const;
        const std::string& getGameName() const;
        const int getUserLimit() const;
        const int getUserNum() const;
        
        const std::list<std::shared_ptr<User>>& getUserList();

        json toJson() const override;
        json getSessionInfo() const ;


        std::string excute();
    private:
        static int getCount();
    private:
        static int count;
        static locker c_lock; 
        int sessionId;
        locker s_lock;
        std::list<std::shared_ptr<User>> userList;
        int maxUsers;
        int numUsers;
        SESSION_STATUS status;
        std::unique_ptr<Game> gameInstance;


};


#endif