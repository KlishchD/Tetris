#pragma once

#include "GlobalDefines.h"

class Tetris
{
public:
    Tetris(std::vector<Grid> grids): m_Grids(std::move(grids))
    {
    }
    
    const Grid& GetGrid(int32_t rotation) const { return m_Grids[rotation % 4]; }
private:
    std::vector<Grid> m_Grids;
};