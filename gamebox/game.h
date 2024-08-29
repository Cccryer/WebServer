#ifndef GAME_H
#define GAME_H
#include<string>
#include "jsoner.h"

enum class GAMETYPE{
    GOMOKU,
    SNAKE
};

constexpr GAMETYPE allGameTypes[] = {
    GAMETYPE::GOMOKU,
    GAMETYPE::SNAKE
};

class Game{
    public:
        Game(std::string name, int plimit,  std::string rule = ""):
            gameName(name), gameRule(rule), maxPlayers(plimit){
                printf("new Game:%s\n", gameName.c_str());
            }

        virtual ~Game() = default;
        virtual void play() = 0;
        virtual void end() = 0;
        virtual void init() = 0;
        virtual json toJson() const = 0;

        virtual std::string excute() = 0;

        const std::string& getName() const;
        const std::string& getRule() const;
        const GAMETYPE getType() const;
        const int getPlayerLimit() const;

    protected:
        std::string gameName;
        std::string gameRule;
        GAMETYPE gameType;
        int maxPlayers;
        

};

#endif
