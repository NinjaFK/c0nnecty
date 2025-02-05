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
    if (board.turn == 2)
        return -sum;
    return sum;
}

int negamax(Board board, int depth, int ply, Stack *stack, SearchSettings &settings);

// minimaxRoot
std::pair<Move, int> negamaxRoot(Board board, int hardStop)
{
    auto start = std::chrono::high_resolution_clock::now();
    int depth = 1;
    Move bestMove = Move(0, 0);
    int value = 0;
    std::chrono::time_point<std::chrono::high_resolution_clock> endTime = start + std::chrono::milliseconds(hardStop);

    SearchSettings settings;
    settings.timeOut = false;
    settings.endTime = endTime;

    Stack stack[50];
    while (true)
    {

        value = negamax(board, depth, 0, stack, settings);
        if (!settings.timeOut)
        {
            std::cout << "I get to depth: " << depth << '\n';
            bestMove = stack[0].pv.moves[0];
            std::cout << "BestMove: " << bestMove << '\n';
        }

        if (settings.timeOut || depth > 50)
        {
            std::cout << "[DEBUG] Stopping iterative deepening. Timeout or max depth reached.\n";
            break;
        }

        depth++;
    }

    // std::vector<Move> moves = board.getmoves();
    // int bestMoveValue = -999999;
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
        std::cout << "[DEBUG] Timeout occurred during negamax.\n";
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
        value = -negamax(cboard, depth - 1, ply + 1, stack, settings);
        if (settings.timeOut)
        {
            return 0;
        }
        if (value > bestMoveValue)
        {
            stack[ply].pv.moves.clear();
            stack[ply].pv.moves.push_back(moves[i]);
            bestMoveValue = value;
        }
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

        if (game.turn == 1)
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
            bestMove = negamaxRoot(game, 5000);
            game.makeMove(bestMove.first);
            std::cout << "eval: " << evalFunction(game) << '\n';
        }
    }
}