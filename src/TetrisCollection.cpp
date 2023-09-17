#include "TetrisCollection.h"

const TetrisCollection& TetrisCollection::Get()
{
    static TetrisCollection collection;
    return collection;
}

const Tetris& TetrisCollection::GetRandomTetris() const
{
    const int32_t index = rand() % m_Tetrises.size();
    return m_Tetrises[index];
}

TetrisCollection::TetrisCollection()
{
    std::vector<Grid> line = {
        {
            { false, true, false, false },
            { false, true, false, false },
            { false, true, false, false },
            { false, true, false, false },
        },
        {
            { false, false, false, false },
            { false, false, false, false },
            { false, false, false, false },
            { true,  true,  true,  true  },
        },
    };
    std::vector<Grid> square = {
        {
            { false, true,  true  },
            { false, true,  true  },
            { false, false, false }
        }
    };
    std::vector<Grid> L = {
        {
            { false, false, true  },
            { true,  true,  true  },
            { false, false, false }
        },
        {
            { false, true, false },
            { false, true, false },
            { false, true, true  },
        },
        {
            { false, false, false },
            { true,  true,  true  },
            { true,  false, false }
        },
        {
            { true,  true, false },
            { false, true, false },
            { false, true, false }
        }
    };

    m_Tetrises.emplace_back(line);
    m_Tetrises.emplace_back(square);
    m_Tetrises.emplace_back(L);
}
