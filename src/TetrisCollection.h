#pragma once

#include "Tetris.h"

class TetrisCollection
{
public:
    static TetrisCollection& Get()
    {
        static TetrisCollection collection;
        return collection;
    }

    const Tetris& GetRandomTetris() const
    {
        const int32_t index = rand() % m_Tetrises.size();
        return m_Tetrises[index];
    }
    
private:
    TetrisCollection()
    {
        std::vector<Grid> line = {
            {
                { GFS_Free,  GFS_Taken, GFS_Free,  GFS_Free },
                { GFS_Free,  GFS_Taken, GFS_Free,  GFS_Free },
                { GFS_Free,  GFS_Taken, GFS_Free,  GFS_Free },
                { GFS_Free,  GFS_Taken, GFS_Free,  GFS_Free },
            },
            {
                { GFS_Free,  GFS_Free,  GFS_Free,  GFS_Free  },
                { GFS_Free,  GFS_Free,  GFS_Free,  GFS_Free  },
                { GFS_Free,  GFS_Free,  GFS_Free,  GFS_Free  },
                { GFS_Taken, GFS_Taken, GFS_Taken, GFS_Taken },
            },
            {
                { GFS_Free,  GFS_Taken, GFS_Free,  GFS_Free },
                { GFS_Free,  GFS_Taken, GFS_Free,  GFS_Free },
                { GFS_Free,  GFS_Taken, GFS_Free,  GFS_Free },
                { GFS_Free,  GFS_Taken, GFS_Free,  GFS_Free },
            },
            {
                { GFS_Free,  GFS_Free,  GFS_Free,  GFS_Free  },
                { GFS_Free,  GFS_Free,  GFS_Free,  GFS_Free  },
                { GFS_Free,  GFS_Free,  GFS_Free,  GFS_Free  },
                { GFS_Taken, GFS_Taken, GFS_Taken, GFS_Taken },
            },
        };
        std::vector<Grid> square = {
            {
                { GFS_Taken,  GFS_Taken },
                { GFS_Taken,  GFS_Taken }
            },
            {
                { GFS_Taken,  GFS_Taken },
                { GFS_Taken,  GFS_Taken }
            },
            {
                { GFS_Taken,  GFS_Taken },
                { GFS_Taken,  GFS_Taken }
            },
            {
                { GFS_Taken,  GFS_Taken },
                { GFS_Taken,  GFS_Taken }
            },
        };

        m_Tetrises.emplace_back(line);
        m_Tetrises.emplace_back(square);
    }

    std::vector<Tetris> m_Tetrises;
};