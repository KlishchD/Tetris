#pragma once

#include "GlobalDefines.h"

class Tetris
{
public:
    Tetris() {}
    Tetris(std::vector<Grid> grids);

    const Grid& GetGrid(int32_t rotation) const;

    int32_t GetHeight(int32_t rotation) const;
private:
    std::vector<Grid> m_Grids;
};