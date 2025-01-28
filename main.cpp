#include "include.h"

struct PV
{
    std::vector<Move> moves;
};

struct Stack
{
    PV pv;
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

int negamax(Board board, int depth);

// minimaxRoot
Move negamaxRoot(Board board, int depth)
{
    std::vector<Move> moves = board.getmoves();
    Move bestMove = Move(0, 0);
    int bestMoveValue = -999999;
    int value = 0;

    for (int i = 0; i < moves.size(); i++)
    {
        Board cboard = board;
        cboard.makeMove(moves[i]);
        std::cout << "moves[i]: " << moves[i] << '\n';
        value = -negamax(cboard, depth - 1);
        std::cout << "value: " << value << '\n';
        if (value > bestMoveValue)
        {
            bestMove = moves[i];
            bestMoveValue = value;
        }
    }
    return bestMove;
}

// minimax
int negamax(Board board, int depth)
{
    if (board.over == 1)
    {
        return -10000;
    }
    if (board.isBoardFull())
    {
        return 0;
    }

    if (depth == 0)
    {
        return evalFunction(board);
    }

    std::vector<Move> moves = board.getmoves();
    int bestMoveValue = -999999;
    int value = 0;

    for (int i = 0; i < moves.size(); i++)
    {
        Board cboard = board;
        cboard.makeMove(moves[i]);
        value = -negamax(cboard, depth - 1);
        bestMoveValue = std::max(value, bestMoveValue);
    }
    return bestMoveValue;
}

void playbot();

int main()
{

    std::string input;
    std::getline(std::cin, input);

    if (input == "ugi")
        UGI();
    if (input == "playbot")
        playbot();
}

void playbot()
{
    Board game("7/7/7/7/7/7 1");
    bool stillPlaying = false;
    int pos = 0;
    Move move = Move(0, 0);

    while (true)
    {
        game.displayBoard();
        std::cout << '\n';
        if (game.over)
        {
            std::cout << "Game over ";
            if (game.turn)
            {
                std::cout << "Yellow wins\n";
            }
            else
            {
                std::cout << "Red wins\n";
            }

            break;
        }
        if (game.isBoardFull())
        {
            std::cout << "Board Full\n";
            break;
        }

        // check if game is over

        if (game.turn == 2)
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
            std::cout << "eval: " << evalFunction(game) << '\n';
        }
        else
        {
            move = negamaxRoot(game, 5);
            game.makeMove(move);
            std::cout << "eval: " << evalFunction(game) << '\n';
        }
    }
}