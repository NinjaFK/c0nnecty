#pragma once
#include "include.h"

[[nodiscard]] inline std::vector<std::string> splitString(const std::string &string,
                                                          const char &delimiter)
{
    std::stringstream string_stream(string);
    std::string segment;
    std::vector<std::string> seglist;

    while (std::getline(string_stream, segment, delimiter))
        seglist.emplace_back(segment);

    return seglist;
}

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
static const std::string reset = "\033[0m";
static const std::string r = "\033[31m";
static const std::string y = "\033[33m";
static const std::string b = "\033[34m";
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
    int board[6][7] = {0};
    int turn;
    int over;
    std::vector<Move> history;

    Board(std::string fen)
    {
        over = 0;
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 7; j++)
            {
                board[i][j] = 0;
            }
        }
        setFen(fen);
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

    void setFen(std::string fen)
    {
        // Samples
        // position fen 7/7/7/7/7/7 0

        std::vector<std::string> params = splitString(fen, ' ');

        std::vector<std::string> pos = splitString(params[0], '/');

        for (int i = 0; i < 6; i++)
        {
            int col = 0;
            for (int j = 0; j < pos[i].size(); j++)
            {
                if (std::isdigit(pos[i][j]))
                {
                    col += pos[i][j] - '0';
                }
                else
                {
                    board[i][col] = (pos[i][j] == 'r') ? 1 : 2;
                    col++;
                }
            }
        }
        turn = params[1][0] - '0';
    }

    bool isBoardFull()
    {
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 7; j++)
            {
                if (board[i][j] == 0)
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
            // std::cout << "board[" << x << "][" << y << "] == board[" << orig_x << "][" << orig_y << "] = " << (board[x][y] == board[orig_x][orig_y]) << '\n';
            if (y < 0 || y > 6)
            {
                break;
            }
            if (x < 0 || x > 5)
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
            // std::cout << "pos: " << check_consecutive(x, y, dx, dy, side) << '\n';
            //  std::cout << "pos2: " << check_consecutive(x, y, -dx, -dy, side) << '\n';
            // std::cout << x << ' ' << y << ' ' << -dx << ' ' << -dy << ' ' << side << '\n';
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
            over = 1;
            return toMake.side;
        }

        return 0;
    }
};
