#include "include.h"

const int INF = 999999;

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

int negamax(Board board, int depth, int alpha, int beta, int ply, Stack *stack, SearchSettings &settings);

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

    if (board.over == 1)
    {
        std::cout << "[DEBUG] Game already over. Returning evaluation.\n";
        return {Move(-1, -1), -10000}; // Losing evaluation
    }

    if (board.isBoardFull())
    {
        std::cout << "[DEBUG] Board is full. Returning draw evaluation.\n";
        return {Move(-1, -1), 0}; // Draw evaluation
    }

    Stack stack[50];
    while (true)
    {

        value = negamax(board, depth, -INF, INF, 0, stack, settings);
        if (!settings.timeOut)
        {
            auto elapsed = std::chrono::high_resolution_clock::now() - start;
            int u = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

            std::cout << "info depth " << depth << " score " << value << " time " << u << " pv ";
            for (auto move : stack[0].pv.moves)
            {
                std::cout << move << " ";
            }
            std::cout << std::endl;
            bestMove = stack[0].pv.moves[0];
        }

        if (settings.timeOut || depth > 50)
        {
            // std::cout << "[DEBUG] Stopping iterative deepening. Timeout or max depth reached.\n";
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
int negamax(Board board, int depth, int alpha, int beta, int ply, Stack *stack, SearchSettings &settings)
{
    if (board.over == 1)
    {
        if (board.checkWin() == board.turn)
        {
            return 10000 - ply;
        }
        else
        {
            return -(10000 - ply);
        }
    }
    if (board.isBoardFull())
    {
        return 0;
    }

    if (depth == 0)
    {
        return evalFunction(board);
    }

    if (std::chrono::high_resolution_clock::now() >= settings.endTime)
    {
        settings.timeOut = true;
        // std::cout << "[DEBUG] Timeout occurred during negamax.\n";
        return 0;
    }

    std::vector<Move> moves = board.getmoves();
    int bestMoveValue = -INF;
    int value = 0;
    int alphaOrig = alpha;

    for (int i = 0; i < moves.size(); i++)
    {
        stack[ply + 1].pv.moves.clear();
        Board cboard = board;
        cboard.makeMove(moves[i]);
        value = -negamax(cboard, depth - 1, -beta, -alpha, ply + 1, stack, settings);
        if (settings.timeOut)
        {
            return 0;
        }
        if (value > bestMoveValue)
        {
            stack[ply].pv.moves.clear();
            stack[ply].pv.moves.push_back(moves[i]);
            if (stack[ply + 1].pv.moves.size() >= 1)
            {
                stack[ply].pv.moves.insert(stack[ply].pv.moves.end(), stack[ply + 1].pv.moves.begin(), stack[ply + 1].pv.moves.end());
            }
            bestMoveValue = value;
        }
        alpha = std::max(alpha, value);
        if (alpha >= beta)
        {
            break;
        }
    }
    return bestMoveValue;
}

void playbot();

void generateRandomFEN(int moveCount, int numPositions, const std::string &filename)
{
    std::ofstream outfile(filename);
    if (!outfile)
    {
        std::cerr << "Error: Could not open file for writing.\n";
        return;
    }

    srand(time(0));

    for (int i = 0; i < numPositions; i++)
    {
        Board game("7/7/7/7/7/7 1");
        for (int j = 0; j < moveCount; j++)
        {
            std::vector<Move> moves;
            moves = game.getmoves();
            if (moves.empty())
                break;
            Move move = moves[rand() % moves.size()];

            Board cboard = game;
            cboard.makeMove(move);
            if (!cboard.over)
            {
                game.makeMove(move);
            }
            else
            {
                j--;
            }
        }
        std::string fen = game.toFEN();
        outfile << fen << "\n";
    }
    outfile.close();
    std::cout << "Saved " << numPositions << " FEN positions to " << filename << ".\n";
}

int main()
{

    std::string input;
    std::getline(std::cin, input);

    if (input == "ugi")
        UGI();
    if (input == "playbot")
        playbot();
    if (input == "gen")
    {
        int moveCount = 6;        // Number of moves to play
        int numPositions = 10000; // Number of FENs to generate
        std::string filename = "openings.txt";

        generateRandomFEN(moveCount, numPositions, filename);
    }
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
            if (game.turn == 1)
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
                if ((pos >= 0 && pos < 7) && game.board[0][pos] == 0)
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