#include "gomoku.h"
#include<iostream>

GomokuGame::GomokuGame(int bs):
    Game("gomoku", 2), boardSize(bs)
    {
        gameType = GAMETYPE::GOMOKU;
        init();
    }


void GomokuGame::init()
{
    board = std::vector<std::vector<char>>(boardSize, std::vector<char>(boardSize, '.'));
    currentPlayer = 'X';
    gameOver = false;
}


void GomokuGame::play()
{
    int x, y;
    while (!gameOver) {
        printBoard();
        std::cout << "Player " << currentPlayer << ", enter your move (row and column): ";
        std::cin >> x >> y;

        if (x < 0 || x >= boardSize || y < 0 || y >= boardSize || board[x][y] != '.') {
            std::cout << "Invalid move. Try again." << std::endl;
            continue;
        }

        board[x][y] = currentPlayer;
        if (checkWin(x, y)) {
            printBoard();
            std::cout << "Player " << currentPlayer << " wins!" << std::endl;
            gameOver = true;
        } else {
            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        }
    }
}


void GomokuGame::end()
{
    std::cout << "Game over. Thank you for playing Gomoku!" << std::endl;
}


json GomokuGame::toJson() const
{
    return json{};
}


void GomokuGame::printBoard() const{
    for (const auto& row : board) {
        for (char cell : row) {
            std::cout << cell << ' ';
        }
        std::cout << std::endl;
    }
}


bool GomokuGame::checkWin(int x, int y) const
{
    return checkDirection(x, y, 1, 0) || checkDirection(x, y, 0, 1) ||
               checkDirection(x, y, 1, 1) || checkDirection(x, y, 1, -1);
}


bool GomokuGame::checkDirection(int x, int y, int dx, int dy) const
{
        int count = 0;
        char target = board[x][y];

        for (int i = -4; i <= 4; ++i) {
            int nx = x + i * dx;
            int ny = y + i * dy;
            if (nx >= 0 && nx < boardSize && ny >= 0 && ny < boardSize && board[nx][ny] == target) {
                count++;
                if (count == 5) return true;
            } else {
                count = 0;
            }
        }
        return false;
}

std::string GomokuGame::excute()
{
    return std::string();
}