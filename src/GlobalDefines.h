#pragma once
#include <cstdint>
#include <vector>

enum GridFieldStatus: uint8_t
{
    GFS_Taken,
    GFS_Free
};

using Grid = std::vector<std::vector<GridFieldStatus>>;
