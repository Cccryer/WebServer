#ifndef GOMOKU_H
#define GOMOKU_H

#include "game.h"
#include<vector>
#include "jsoner.h"
class GomokuGame : public Game {
public:
    GomokuGame(int bs = 15);

    void init() override;

    void play() override;

    void end() override;

    json toJson() const override;

    std::string excute() override;

private:
    int boardSize;
    std::vector<std::vector<char>> board;
    char currentPlayer;
    bool gameOver;

    void printBoard() const;

    bool checkWin(int x, int y) const;

    bool checkDirection(int x, int y, int dx, int dy) const;
};

#endif
