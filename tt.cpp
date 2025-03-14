#include "include.h"
struct TTEntry
{
    uint64_t hash = 0;
    Move bestMove = Move(0, 0);
};

struct TransTable
{
private:
    std::vector<TTEntry> table;
    uint32_t numEntries;

public:
    TransTable(uint32_t mega = 0)
    {
        numEntries = (mega * 1024) / (sizeof(TTEntry) * 1024);
        if (mega != 0)
        {
            table.resize(numEntries);
        }
    }
};