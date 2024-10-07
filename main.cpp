#include <iostream>
#include <vector>
#include <cmath>

class Move
{
public:
};

class Board
{
public:
    int board[9];
    int turn;
    std::vector<Move> history;
};

// eval
int evalFunction(Board board)
{
}

int minimax(Board board, int depth);

// minimaxRoot
Move minimaxRoot(Board board, int depth)
{
}

// minimax
int minimax(Board board, int depth)
{
}

int main()
{
    Board game;
    bool stillPlaying = false;
    int pos = 0;

    while (true)
    {
    }
}