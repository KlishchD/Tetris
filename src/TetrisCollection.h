#pragma once

#include "Tetris.h"

class TetrisCollection
{
public:
    static const TetrisCollection& Get();

    const Tetris& GetRandomTetris() const;
    
private:
    TetrisCollection();

    std::vector<Tetris> m_Tetrises;
};