#pragma once

#include "Tetris.h"
#include "Vector.h"

struct MapTile
{
    bool IsCollisionTaken = false;
    int32_t RenderColorId = 0;

    void Reset()
    {
        IsCollisionTaken = false;
        RenderColorId = 0;
    }

    MapTile& operator=(const MapTile& tile) = default;
};

using MapGrid = std::vector<std::vector<MapTile>>;

class Map
{
public:
    Map(int32_t sizeX, int32_t sizeY);

    Map& operator=(Map& right);

    bool WillCollide(Vector2i position, const Tetris& tetris, int32_t rotation) const;

    void AddTetrisForRendering(Vector2i position, const Tetris& tetris, int32_t rotation, int32_t ColorId);
    void RemoveTetrisFromRendering(Vector2i position, const Tetris& tetris, int32_t rotation);

    void AddTetris(Vector2i position, const Tetris& tetris, int32_t rotation, int32_t ColorId);
    void RemoveTetris(Vector2i position, const Tetris& tetris, int32_t rotation);

    bool CanMoveTo(Vector2i to, const Tetris& tetris, int32_t rotation) const;

    int32_t DoLineCheck();
    void DropLine();
    
    const MapGrid& GetMap() const;
    int32_t GetSizeX() const;
    int32_t GetSizeY() const;

    void Reset();
private:
    int32_t m_SizeX;
    int32_t m_SizeY;
    MapGrid m_Map;

    int32_t m_SquareSize = 20.0;
};
