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
const int evalTable[6][7] = {
    {3, 4, 5, 7, 5, 4, 3},
    {4, 6, 8, 10, 8, 6, 4},
    {5, 8, 11, 13, 11, 8, 5},
    {5, 8, 11, 13, 11, 8, 5},
    {4, 6, 8, 10, 8, 6, 4},
    {3, 4, 5, 7, 5, 4, 3}};
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
        std::cout << reset;
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

    int check_consecutive(int x, int y, int dx, int dy, int side)
    {
        int consecutive = 0;
        int orig_x = x;
        int orig_y = y;
        // std::cout << board[orig_x][orig_y] << std::endl;
        for (int i = 0; i < 3; i++)
        {
            x += dx;
            y += dy;
            if (y < 0 || y > 5)
            {
                break;
            }
            if (x < 0 || x > 6)
            {
                break;
            }
            if (board[x][y] == board[orig_x][orig_y])
            {
                consecutive++;
            }
            else
            {
                break;
            }
        }
        return consecutive;
    }

    bool checkWin(int x, int y, int side)
    {
        const std::vector<std::pair<int, int>> directions = {{0, 1}, {1, 0}, {1, 1}, {-1, 1}};
        for (const auto &[dx, dy] : directions)
        {
            if ((check_consecutive(x, y, dx, dy, side) + check_consecutive(x, y, -dx, -dy, side)) >= 3)
            {
                return true;
            }
        }
        return 0;
    }
    int makeMove(Move toMake)
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
        board[count][toMake.pos] = toMake.side;
        turn = (turn == 1) ? 2 : 1;

        // winstate
        if (checkWin(count, toMake.pos, toMake.side))
        {
            std::cout << "Player " << toMake.side << " wins!" << std::endl;
            return toMake.side;
        }

        return 0;
    }
};

// eval
int evalFunction(Board board)
{
    int sum = 0;
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            if (board.board[i][j] == 1)
            {
                sum += evalTable[i][j];
            }
            else if (board.board[i][j] == 2)
            {
                sum -= evalTable[i][j];
            }
        }
    }
    return sum;
}

int minimax(Board board, int depth);

// minimaxRoot
Move minimaxRoot(Board board, int depth)
{
    Move moves(1, 1);
    return moves;
}

// minimax
int minimax(Board board, int depth)
{
    return 0;
}

int main()
{
    Board game;
    bool stillPlaying = false;
    int pos = 0;
    Move move = Move(0, 0);

    while (true)
    {
        game.displayBoard();

        // check if game is over

        if (true)
        {
            std::cout << "Your move: ";
            while (true)
            {
                std::cin >> pos;
                if (game.board[0][pos] == 0)
                {
                    break;
                }
                else
                {
                    std::cout << "Please choose valid move: ";
                }
            }
            game.makeMove(Move(pos, game.turn));
        }
        else
        {
            // move = negamaxRoot(game, 7);
            game.makeMove(move);
        }
    }
}