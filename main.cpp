#include "include.h"

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
std::pair<Move, int> negamaxRoot(Board board, int hardStop)
{
    std::vector<Move> moves = board.getmoves();
    auto start = std::chrono::high_resolution_clock::now();
    int depth = 1;
    Move bestMove = Move(0, 0);
    int bestMoveValue = -999999;
    int value = 0;
    std::chrono::time_point<std::chrono::high_resolution_clock> endTime = start + std::chrono::milliseconds(hardStop);

    SearchSettings settings;
    settings.timeOut = false;
    settings.endTime = endTime;

    while (true)
    {
        Stack stack[256];

        value = -negamax(board, depth, 0, stack, settings);

        if (!settings.timeOut)
        {
            bestMove = stack[0].pv.moves[0];
        }

        if (depth > 256)
            break;

        depth++;
    }

    // for (int i = 0; i < moves.size(); i++)
    // {
    //     Board cboard = board;
    //     cboard.makeMove(moves[i]);
    //     std::cout << "moves[i]: " << moves[i] << '\n';
    //     value = -negamax(cboard, depth - 1);
    //     std::cout << "value: " << value << '\n';
    //     if (value > bestMoveValue)
    //     {
    //         bestMove = moves[i];
    //         bestMoveValue = value;
    //     }
    // }
    return {bestMove, value};
}

// minimax
int negamax(Board board, int depth, int ply, Stack *stack, SearchSettings &settings)
{
    if (board.over == 1)
    {
        return -10000;
    }
    if (board.isBoardFull())
    {
        return 0;
    }

    if (std::chrono::high_resolution_clock::now() >= settings.endTime)
    {
        settings.timeOut = true;
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
        stack[ply + 1].pv.moves.clear();
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

            std::pair<Move, int> bestMove;
            bestMove = negamaxRoot(game, 5);
            game.makeMove(bestMove.first);
            std::cout << "eval: " << evalFunction(game) << '\n';
        }
    }
}