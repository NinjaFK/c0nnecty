#include "include.h"

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
        }
    }
}

int main()
{
    int moveCount = 6;     // Number of moves to play
    int numPositions = 10; // Number of FENs to generate
    std::string filename = "openings.txt";

    generateRandomFEN(moveCount, numPositions, filename);
    return 0;
}