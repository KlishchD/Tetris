#include "Map.h"

Map::Map(int32_t sizeX, int32_t sizeY): m_SizeX(sizeX), m_SizeY(sizeY),
    m_Map(sizeY, std::vector<MapTile>(sizeX))
{
        
}


bool Map::WillCollide(Vector2i position, const Tetris& tetris, int32_t rotation) const
{
    position.y++;
    const Grid& grid = tetris.GetGrid(rotation);
        
    for (int32_t y = 0; y < grid.size(); ++y)
    {
        for (int32_t x = 0; x < grid[y].size(); ++x)
        {
            if (grid[y][x] && (position.y + y >= m_SizeY || m_Map[position.y + y][position.x + x].IsCollisionTaken))
            {
                return true;
            }
        }
    }
    return false;
}

void Map::AddTetrisForRendering(Vector2i position, const Tetris& tetris, int32_t rotation, int32_t ColorId)
{
    const Grid& grid = tetris.GetGrid(rotation);
        
    for (int32_t y = 0; y < grid.size(); ++y)
    {
        for (int32_t x = 0; x < grid[y].size(); ++x)
        {
            if (grid[y][x])
            {
                m_Map[position.y + y][position.x + x].RenderColorId = ColorId;
            }
        }
    }
}

void Map::RemoveTetrisFromRendering(Vector2i position, const Tetris& tetris, int32_t rotation)
{
    const Grid& grid = tetris.GetGrid(rotation);
        
    for (int32_t y = 0; y < grid.size(); ++y)
    {
        for (int32_t x = 0; x < grid[y].size(); ++x)
        {
            if (grid[y][x])
            {
                m_Map[position.y + y][position.x + x].RenderColorId = 0;
            }
        }
    }
}

void Map::RemoveTetris(Vector2i position, const Tetris& tetris, int32_t rotation)
{
    const Grid& grid = tetris.GetGrid(rotation);
        
    for (int32_t y = 0; y < grid.size(); ++y)
    {
        for (int32_t x = 0; x < grid[y].size(); ++x)
        {
            if (grid[y][x])
            {
                m_Map[position.y + y][position.x + x].Reset();
            }
        }
    }
}

void Map::AddTetris(Vector2i position, const Tetris& tetris, int32_t rotation, int32_t ColorId)
{
    const Grid& grid = tetris.GetGrid(rotation);
        
    for (int32_t y = 0; y < grid.size(); ++y)
    {
        for (int32_t x = 0; x < grid[y].size(); ++x)
        {
            if (grid[y][x])
            {
                m_Map[position.y + y][position.x + x].IsCollisionTaken = true;
                m_Map[position.y + y][position.x + x].RenderColorId = ColorId;
            }
        }
    }
}

const MapGrid& Map::GetMap() const
{
    return m_Map;
}

bool Map::CanMoveTo(Vector2i to, const Tetris& tetris, int32_t rotation) const
{
    const Grid& grid = tetris.GetGrid(rotation);

    for (int32_t y = 0; y < grid.size(); ++y)
    {
        for (int32_t x = 0; x < grid[y].size(); ++x)
        {
            if (grid[y][x] && (to.x + x < 0 || to.x + x >= m_SizeX || to.y + y < 0 || to.y + y >= m_SizeY))
            {
                return false;
            }
        }
    }
        
    return !WillCollide(to, tetris, rotation);
}

int32_t Map::DoLineCheck()
{
    int32_t LinesCount = 0;
        
    for (int32_t y = m_SizeY - 1; y >= 0; --y)
    {
        bool IsLine = true;
        for (int32_t x = 0; x < m_SizeX; ++x)
        {
            IsLine = IsLine && (m_Map[y][x].IsCollisionTaken);
        }
            
        LinesCount += IsLine;
    }
    
    return LinesCount;
}

void Map::DropLine()
{
    int32_t LineRow = m_SizeY - 1;
    
    for (int32_t y = 0; y <= m_SizeY; ++y)
    {
        bool IsLine = true;
        for (int32_t x = 0; x < m_SizeX; ++x)
        {
            IsLine = IsLine && (m_Map[y][x].IsCollisionTaken);
        }
        
        if (IsLine)
        {
            LineRow = y;
            break;
        }
    }
    
    for (int32_t y = LineRow; y > 0; --y)
    {
        bool IsLine = true;
        for (int32_t x = 0; x < m_SizeX; ++x)
        {
            IsLine = IsLine && (m_Map[y][x].IsCollisionTaken);
        }
        
        for (int32_t x = 0; x < m_SizeX; ++x)
        {
            m_Map[y][x] = m_Map[y - 1][x];
        }
    }
    
    for (int32_t x = 0; x < m_SizeX; ++x)
    {
        m_Map[0][x].Reset();
    }
}

Map& Map::operator=(Map& right)
{
    m_SizeX = right.m_SizeX;
    m_SizeY = right.m_SizeY;
    m_Map = right.m_Map;
    m_SquareSize = right.m_SquareSize;
    return *this;
}

void Map::Reset()
{
    for (auto row: m_Map)
    {
        for (auto item: row)
        {
            item.Reset();
        }
    }
}

int32_t Map::GetSizeX() const
{
    return m_SizeX;
}

int32_t Map::GetSizeY() const
{
    return m_SizeY;
}
