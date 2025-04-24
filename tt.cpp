#include "include.h"

struct TTEntry
{
    uint64_t hash = 0;
    Move bestMove = Move(0, 0);
    uint8_t depth = 0;
    uint32_t score = 0;
};

struct TransTable
{
private:
    std::vector<TTEntry> table;
    uint32_t numEntries;

public:
    TransTable(uint32_t mega = 16)
    {
        numEntries = (mega * 1024) / (sizeof(TTEntry)) * 1024;
        if (mega != 0)
        {
            table.resize(numEntries);
        }
    }

    TTEntry probe(uint32_t hash)
    {
        return table[hash % numEntries];
    }

    void insert(TTEntry entry)
    {
        table[entry.hash % numEntries] = entry;
    }

    void clear()
    {
        for (int i = 0; i < numEntries; i++)
        {
            table[i] = TTEntry();
        }
    }
};