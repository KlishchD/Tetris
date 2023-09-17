#include "Tetris.h"

Tetris::Tetris(std::vector<Grid> grids): m_Grids(std::move(grids))
{
}

const Grid& Tetris::GetGrid(int32_t rotation) const
{
    return m_Grids[rotation % m_Grids.size()];
}

int32_t Tetris::GetHeight(int32_t rotation) const
{
    int32_t minHeight = INT32_MAX;
    int32_t maxHeight = INT32_MIN;
    
    for (int32_t i = 0; i < m_Grids[rotation].size(); ++i)
    {
        for (int32_t j = 0; j < m_Grids[rotation][i].size(); ++j)
        {
            if (m_Grids[rotation][i][j])
            {
                maxHeight = i;
                minHeight = std::min(minHeight, i);
            }
        }
    }

    return maxHeight - minHeight;
}
