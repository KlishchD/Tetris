#pragma once

#include "TetrisCollection.h"
#include "Vector.h"

class Map
{
public:
    Map(size_t sizeX, int32_t sizeY): m_SizeX(sizeX), m_SizeY(sizeY),
                                      m_Map(sizeY, std::vector<GridFieldStatus>(sizeX, GFS_Free))
    {
        
    }

    bool WillCollide(Vector2i position, const Tetris& tetris, int32_t rotation) const
    {
        position.y++;
        const Grid& grid = tetris.GetGrid(rotation);
        
        for (size_t y = 0; y < grid.size(); ++y)
        {
            for (size_t x = 0; x < grid[y].size(); ++x)
            {
                if (grid[y][x] == GFS_Taken && (position.y + y >= m_SizeY || m_Map[position.y + y][position.x + x] == GFS_Taken))
                {
                    return true;
                }
            }
        }
        return false;
    }

    bool IsAlive(Vector2i position, const Tetris& tetris, int32_t rotation)
    {
        const Grid& grid = tetris.GetGrid(rotation);
        
        for (size_t y = 0; y < grid.size(); ++y)
        {
            for (size_t x = 0; x < grid[y].size(); ++x)
            {
                if (grid[y][x] == GFS_Taken && position.y + y <= 0)
                {
                    return false;
                }
            }
        }
        return true;
    }

    void RemoveTetris(Vector2i position, const Tetris& tetris, int32_t rotation)
    {
        const Grid& grid = tetris.GetGrid(rotation);
        
        for (size_t y = 0; y < grid.size(); ++y)
        {
            for (size_t x = 0; x < grid[y].size(); ++x)
            {
                if (grid[y][x] == GFS_Taken)
                {
                    m_Map[position.y + y][position.x + x] = GFS_Free;
                }
            }
        }
    }
    
    void AddTetris(Vector2i position, const Tetris& tetris, int32_t rotation)
    {
        const Grid& grid = tetris.GetGrid(rotation);
        
        for (size_t y = 0; y < grid.size(); ++y)
        {
            for (size_t x = 0; x < grid[y].size(); ++x)
            {
                if (grid[y][x] == GFS_Taken)
                {
                    m_Map[position.y + y][position.x + x] = GFS_Taken;
                }
            }
        }
    }

    std::vector<float> GenerateData(Vector2f Scale)
    {
        std::vector<float> result;

        for (float y = 0; y < m_SizeY; ++y)
        {
            for (float x = 0; x < m_SizeX; ++x)
            {
                Vector3f Color = GetColor(x, y);

                result.push_back((m_SizeX - x) * Scale.x);
                result.push_back((m_SizeY - y) * Scale.y);

                result.push_back(Color.x);
                result.push_back(Color.y);
                result.push_back(Color.z);

                result.push_back((m_SizeX - x - 1) * Scale.x);
                result.push_back((m_SizeY - y) * Scale.y);

                result.push_back(Color.x);
                result.push_back(Color.y);
                result.push_back(Color.z);

                result.push_back((m_SizeX - x - 1) * Scale.x);
                result.push_back((m_SizeY - y - 1) * Scale.y);
                
                result.push_back(Color.x);
                result.push_back(Color.y);
                result.push_back(Color.z);

                result.push_back((m_SizeX - x) * Scale.x);
                result.push_back((m_SizeY - y - 1) * Scale.y);

                result.push_back(Color.x);
                result.push_back(Color.y);
                result.push_back(Color.z);

            }
        }

        return result;
    }

    std::vector<Vector2f> GeneratePoints(Vector2f Scale)
    {
        std::vector<Vector2f> result;

        for (float y = 0; y <= m_SizeY; ++y)
        {
            for (float x = 0; x <= m_SizeX; ++x)
            {
                result.emplace_back((m_SizeX - x) * Scale.x, (m_SizeY - y) * Scale.y);
            }
        }
        
        return result;
    }

    Vector3f GetColor(int32_t x, int32_t y)
    {
        x = std::max(0, std::min((int32_t)m_SizeX - 1, x));
        y = std::max(0, std::min((int32_t)m_SizeY - 1, y));
        return (m_Map[y][x] == GFS_Taken) ? Vector3f(1.0f, 0.0f, 0.0f) : Vector3f(1.0f, 1.0f, 1.0f);
    }
    
    std::vector<Vector3f> GenerateColors()
    {
        std::vector<Vector3f> result;
        for (float y = 0; y <= m_SizeY; ++y)
        {
            for (float x = 0; x <= m_SizeX; ++x)
            {
                result.push_back(GetColor(x, y));
            }
        }

        return result;
    }

    std::vector<int32_t> GenerateIndexes()
    {
        std::vector<int32_t> indices;
        
        for (int32_t y = 0; y < m_SizeY; ++y)
        {
            for (int32_t x = 0; x < m_SizeX; ++x)
            {
                indices.emplace_back(x       + y * (m_SizeX + 1));
                indices.emplace_back((x + 1) + y * (m_SizeX + 1));
                indices.emplace_back((x + 1) + (y + 1) * (m_SizeX + 1));
                indices.emplace_back(x       + (y + 1) * (m_SizeX + 1));
            }
        }
        return indices;
    }
    
    const Grid& GetMap() const {return m_Map; }

    bool CanMoveFromTo(Vector2i from, Vector2i to, const Tetris& tetris, int32_t rotation)
    {
        const Grid& grid = tetris.GetGrid(rotation);

        for (size_t y = 0; y < grid.size(); ++y)
        {
            for (size_t x = 0; x < grid[y].size(); ++x)
            {
                if (grid[y][x] == GFS_Taken && (to.x + x < 0 || to.x + x >= m_SizeX || to.y + y < 0 || to.y + y >= m_SizeY))
                {
                    return false;
                }
            }
        }
        
        RemoveTetris(from, tetris, rotation);
        bool result = !WillCollide(to, tetris, rotation);
        AddTetris(from, tetris, rotation);
        return result;
    }
    
private:
    size_t m_SizeX;
    size_t m_SizeY;
    Grid m_Map;
};
