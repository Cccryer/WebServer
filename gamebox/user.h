#ifndef USER_H
#define USER_H
#include<string>
#include<memory>
#include<unordered_map>
#include "jsoner.h"
class GameSession;
enum class GAMETYPE;

class conn;

class User : public JsonSerializable
{
    public:
        User() = default;
        User(std::string n, int i):
            name(n), status(true), id(i){}
        ~User() = default;

        const std::string& getName() const {return name;}
        void setName(const std::string& newname){ name = newname;}

        const int getId() const {return id;}
        void setId(int i) {id = i;}


        const int getScore(std::string);
        void setScore(std::string, int);

        const bool getStatus() const {return status;}
        void setStatus(bool st) { status = st;}

        std::shared_ptr<GameSession> getSession() const;
        void setSession(std::shared_ptr<GameSession>);

        const GAMETYPE getCurGameType();
        void setCurGameType(GAMETYPE);

        json toJson() const override;

        conn* getConn();
    private:
        std::string name;
        int id;
        std::shared_ptr<GameSession> currentSession;
        GAMETYPE currentGameType;
        std::unordered_map<std::string, int> scoresMap;
        bool status;


        conn* connptr;
};

#endif