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
    Move()
    {
        pos = 0;
        side = 1;
    }
    Move(int p, int s)
    {
        pos = p;
        side = s;
    }
    int pos;
    int side;

    friend std::ostream &operator<<(std::ostream &os, const Move &move);
};

struct PV
{
    std::vector<Move> moves;
};

struct Stack
{
    PV pv;
};

struct SearchSettings
{
    std::chrono::time_point<std::chrono::high_resolution_clock> endTime;
    bool timeOut = false;
};

std::ostream &operator<<(std::ostream &os, const Move &move)
{
    if (move.side == 1)
        os << move.pos;
    else
        os << move.pos;
    return os;
}

namespace zobrist
{
    static constexpr uint64_t RANDOM_ARRAY[84] = {
        0x9D39247E33776D41, 0x2AF7398005AAA5C7, 0x44DB015024623547, 0x9C15F73E62A76AE2,
        0x75834465489C0C89, 0x3290AC3A203001BF, 0x0FBBAD1F61042279, 0xE83A908FF2FB60CA,
        0x0D7E765D58755C10, 0x1A083822CEAFE02D, 0x9605D5F0E25EC3B0, 0xD021FF5CD13A2ED5,
        0x40BDF15D4A672E32, 0x011355146FD56395, 0x5DB4832046F3D9E5, 0x239F8B2D7FF719CC,
        0x05D1A1AE85B49AA1, 0x679F848F6E8FC971, 0x7449BBFF801FED0B, 0x7D11CDB1C3B7ADF0,
        0x82C7709E781EB7CC, 0xF3218F1C9510786C, 0x331478F3AF51BBE6, 0x4BB38DE5E7219443,
        0xAA649C6EBCFD50FC, 0x8DBD98A352AFD40B, 0x87D2074B81D79217, 0x19F3C751D3E92AE1,
        0xB4AB30F062B19ABF, 0x7B0500AC42047AC4, 0xC9452CA81A09D85D, 0x24AA6C514DA27500,
        0x4C9F34427501B447, 0x14A68FD73C910841, 0xA71B9B83461CBD93, 0x03488B95B0F1850F,
        0x637B2B34FF93C040, 0x09D1BC9A3DD90A94, 0x3575668334A1DD3B, 0x735E2B97A4C45A23,
        0x18727070F1BD400B, 0x1FCBACD259BF02E7, 0xD310A7C2CE9B6555, 0xBF983FE0FE5D8244,
        0x9F74D14F7454A824, 0x51EBDC4AB9BA3035, 0x5C82C505DB9AB0FA, 0xFCF7FE8A3430B241,
        0x3253A729B9BA3DDE, 0x8C74C368081B3075, 0xB9BC6C87167C33E7, 0x7EF48F2B83024E20,
        0x11D505D4C351BD7F, 0x6568FCA92C76A243, 0x4DE0B0F40F32A7B8, 0x96D693460CC37E5D,
        0x42E240CB63689F2F, 0x6D2BDCDAE2919661, 0x42880B0236E4D951, 0x5F0F4A5898171BB6,
        0x39F890F579F92F88, 0x93C5B5F47356388B, 0x63DC359D8D231B78, 0xEC16CA8AEA98AD76,
        0x5355F900C2A82DC7, 0x07FB9F855A997142, 0x5093417AA8A7ED5E, 0x7BCBC38DA25A7F3C,
        0x19FC8A768CF4B6D4, 0x637A7780DECFC0D9, 0x8249A47AEE0E41F7, 0x79AD695501E7D1E8,
        0x14ACBAF4777D5776, 0xF145B6BECCDEA195, 0xDABF2AC8201752FC, 0x24C3C94DF9C8D3F6,
        0xBB6E2924F03912EA, 0x0CE26C0B95C980D9, 0xA49CD132BFBF7CC4, 0xE99D662AF4243939,
        0x27E6AD7891165C3F, 0x8535F040B9744FF1, 0x54B3F4FA5F40D873, 0x72B12C32127FED2B};
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
    int turn; // 1 for red 2 for yellow
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

    std::string toFEN()
    {
        std::string fen = "";

        for (int i = 0; i < 6; i++)
        {
            int count = 0;
            for (int j = 0; j < 7; j++)
            {
                if (board[i][j] == 0)
                {
                    count++;
                }
                else
                {
                    if (count > 0)
                    {
                        fen += std::to_string(count); // Add empty count to FEN
                        count = 0;
                    }
                    fen += (board[i][j] == 1) ? 'r' : 'y'; // 'r' for Red, 'y' for Yellow
                }
            }
            if (count > 0)
            {
                fen += std::to_string(count); // Final empty spaces in row
            }
            if (i < 5)
                fen += "/"; // Separate rows
        }
        fen += " " + std::to_string(turn); // Append turn
        return fen;
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

    bool pieceCheckWin(int x, int y, int side)
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
        if (pieceCheckWin(count, toMake.pos, toMake.side))
        {
            over = 1;
        }

        return 0;
    }

    int checkWin()
    {
        for (int i = 0; i < 6; i++)
        {
            for (int j = 0; j < 7; j++)
            {
                if (board[i][j] != 0)
                {
                    if (pieceCheckWin(i, j, board[i][j]))
                    {
                        over = 1;
                        return board[i][j];
                    }
                }
            }
        }
        return 0;
    }
};
