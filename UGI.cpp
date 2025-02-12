#include "include.h"

/* [[nodiscard]] inline std::vector<std::string> splitString(const std::string &string,
                                                          const char &delimiter)
{
    std::stringstream string_stream(string);
    std::string segment;
    std::vector<std::string> seglist;

    while (std::getline(string_stream, segment, delimiter))
        seglist.emplace_back(segment);

    return seglist;
} */

void UGI()
{

    std::string command;
    std::cout << "id name c0nnecty" << std::endl;
    std::cout << "id author NinjaFK" << std::endl;
    std::cout << "option name hash type spin default 16" << std::endl;
    std::cout << "ugiok" << std::endl;

    while (true)
    {
        if (!getline(std::cin, command))
            return;
        std::vector<std::string> split = splitString(command, ' ');
        if (split.size() == 0)
            continue;

        if (split[0] == "isready")
        {
            std::cout << "readyok" << std::endl;
            break;
        }
        else if (split[0] == "quit")
            return;
    }

    Board game("7/7/7/7/7/7 1");
    bool stillPlaying = false;
    int pos = 0;
    std::pair<Move, int> negamaxRoot(Board board, int hardStop);

    while (true)
    {
        if (!getline(std::cin, command))
            return;
        std::vector<std::string> split = splitString(command, ' ');
        if (split.size() == 0)
            continue;

        if (split[0] == "isready")
        {
            std::cout << "readyok" << std::endl;
        }

        // position fen 7/7/7/7/7/7 1 moves 2 3 2 2 3 3 3 3 3 0 0 0 0 0 0 2 4 5 4 5 4 4 2 2 5 5 5 6 1 1 1 6 6 4 1 5 1 1 4 6 6 6
        else if (split[0] == "position")
        {
            if (split[1] == "fen")
            {
                std::string fen;
                fen.append(split[2]);
                fen.append(" ");
                fen.append(split[3]);

                game = Board(fen);
            }
            int i = 4;
            if (split.size() > i && split[i] == "moves")
            {
                i++;
                while (split.size() > i)
                {
                    int test = stoi(split[i]);
                    game.makeMove(Move(stoi(split[i]), game.turn));
                    i++;
                }
                std::cout << '\n';
            }
        }

        else if (split[0] == "query")
        {
            std::string str = "false";
            if (split[1] == "p1turn")
            {
                if (!(game.turn - 1))
                {
                    str = "true";
                }
                std::cout << "response " << str << std::endl;
            }
            if (split[1] == "gameover")
            {
                if ((game.over || game.isBoardFull()))
                {
                    str = "true";
                }
                std::cout << "response " << str << std::endl;
            }
            if (split[1] == "result")
            {
                if (!game.checkWin())
                    std::cout << "response none" << std::endl;

                if (game.over)
                {
                    if (game.turn == 2)
                    {
                        std::cout << "response p1win" << std::endl;
                    }
                    else
                    {
                        std::cout << "response p2win" << std::endl;
                    }
                }
                if (game.isBoardFull())
                    std::cout << "reponse draw" << std::endl;
            }
        }

        else if (split[0] == "display")
        {
            game.displayBoard();
        }

        else if (split[0] == "go")
        {
            int mtime = 100;
            std::pair<Move, int> bestMove;

            if (split[1] == "movetime")
            {
                mtime = stoi(split[2]);
                bestMove = negamaxRoot(game, mtime);
            }

            else if (split[1] == "p1time")
            {
                int p1t = 0;
                int p2t = 0;
                int p1inc = 0;
                int p2inc = 0;

                p1t = stoi(split[2]);
                p2t = stoi(split[4]);

                p1inc = stoi(split[6]);
                p2inc = stoi(split[8]);

                if (game.turn = 1)
                {
                    bestMove = negamaxRoot(game, std::min(p1t / 2, (p1t / 20) + p1inc));
                }
                else
                {
                    bestMove = negamaxRoot(game, std::min(p2t / 2, (p2t / 20) + p2inc));
                }
            }

            std::cout << "bestmove " << bestMove.first << std::endl;
        }

        else if (split[0] == "checkwin")
        {
            int winstate = game.checkWin();
            if (winstate)
            {
                if (winstate == 2)
                {
                    std::cout << "Yellow wins\n";
                }
                else
                {
                    std::cout << "Red wins\n";
                }
            }
        }

        else if (split[0] == "quit")
            return;
    }
}