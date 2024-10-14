#include <iostream>
#include <vector>
#include <cmath>

class Move
{
public:
    Move(int p, int s)
    {
        pos = p;
        side = s;
    }
    int pos;
    int side;

    friend std::ostream &operator<<(std::ostream &os, const Move &move);
};

std::ostream &operator<<(std::ostream &os, const Move &move)
{
    if (move.side == 1)
        os << "\033[31m" << move.pos;
    else
        os << "\033[33m" << move.pos;
    return os;
}

class Board
{
public:
    int board[6][7];
    int turn;
    std::vector<Move> history;
    const std::string reset = "\033[0m";
    const std::string r = "\033[31m";
    const std::string y = "\033[33m";
    const std::string b = "\033[34m";

    Board()
    {
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 7; j++)
            {
                board[i][j] = 0;
            }
        }
    }

    // Red is 1
    // Yellow is 2

    void displayBoard()
    {
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 7; j++)
            {
                std::cout << b << "[";
                if (board[i][j] == 1)
                {
                    std::cout << r << 'o';
                }
                else if (board[i][j] == 2)
                {
                    std::cout << y << 'o';
                }
                else
                {
                    std::cout << reset << ' ';
                }
                std::cout << b << "]";
            }
            std::cout << '\n';
        }
    }

    std::vector<Move> getmoves()
    {
        std::vector<Move> moves;

        for (int i = 0; i < 7; i++)
        {
            if (board[0][i] == 0)
            {
                moves.push_back(Move(i, turn));
            }
        }

        /* for (auto it : moves)
        {
            std::cout << it << " \n";
        } */

        return moves;
    }

    bool isBoardFull()
    {
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 7; j++)
            {
                if (board[i][j] = 0)
                {
                    return false;
                }
            }
        }
        return true;
    }

    void makeMove(Move toMake)
    {
        int count = 0;
        history.push_back(toMake);
        for (int i = 1; i < 6; i++)
        {
            if (board[i][toMake.pos] != 0)
            {
                break;
            }
            count++;
        }
        // std::cout << "count: " << count << '\n';
        // std::cout << "tOmake.pos " << toMake.pos << '\n';
        // std::cout << "toMake.side: " << toMake.side << '\n';
        board[count][toMake.pos] = toMake.side;
        turn = (turn == 1) ? 2 : 1;
    }
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
        game.displayBoard();
        std::cin >> pos;
        game.turn = 1;
        game.makeMove(Move(pos, game.turn));
    }
}