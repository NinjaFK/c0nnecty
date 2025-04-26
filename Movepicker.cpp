#include "include.h"

uint16_t TTscore = 10000;
struct MovePicker
{

    int curr = 0;
    std::vector<Move> moves;

    MovePicker(Board board, Move move)
    {
        moves = board.getmoves();

        for (int i = 0; i < moves.size(); i++)
        {
            if ((moves[i].pos == move.pos) && (moves[i].side == move.side))
            {
                moves[i].score = TTscore;
            }
        }
    }

    bool next(Move &move, Board board)
    {
        if (curr == moves.size())
        {
            return false;
        }

        int bestMoveIdx = curr;
        for (int i = curr + 1; i < moves.size(); i++)
        {
            if (moves[i].score > moves[bestMoveIdx].score)
            {
                bestMoveIdx = i;
            }
        }

        move = moves[bestMoveIdx];
        std::swap(moves[curr], moves[bestMoveIdx]);
        curr++;
        return true;
    }
};
