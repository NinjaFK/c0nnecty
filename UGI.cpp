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
            break;
        }

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
                    std::cout << "test " << test << '\n';
                    std::cout << "turn " << game.turn << '\n';
                    game.makeMove(Move(stoi(split[i]), game.turn));
                    i++;
                }
                std::cout << '\n';
            }
        }

        else if (split[0] == "query")
        {
            if (split[1] == "p1turn")
            {
                std::cout << "response " << !(game.turn - 1) << std::endl;
            }
            if (split[1] == "gameover")
            {
                std::cout << "response " << game.over << std::endl;
            }
            if (split[1] == "result")
            {
                if (stillPlaying)
                    std::cout << "response none" << std::endl;

                if (game.over)
                {
                    if (game.turn == 0)
                    {
                        std::cout << "response p1wins" << std::endl;
                    }
                    else
                    {
                        std::cout << "response p2wins" << std::endl;
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

            bestMove = negamaxRoot(game, 7000);

            std::cout << "bestmove " << bestMove.first << '\n';
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