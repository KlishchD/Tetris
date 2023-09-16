#pragma once
#include "GlobalDefines.h"

#include <string>
#include <unordered_map>
#include <glm/ext/matrix_float4x4.hpp>
#include "ColorLibrary.h"
#include "Tetris.h"

class Map;

class Renderer
{
public:
    static Renderer& Get();

    void SetUp(float WindowSizeX, float WindowSizeY, int32_t mapSizeX, int32_t mapSizeY);
    
    void DrawField(const Map& map);
    void DrawNextTetris(const Tetris& tetris, int32_t colorId);
private:
    const ColorLibrary& m_ColorLibrary = ColorLibrary::Get();

    glm::mat4 m_MapModelMatrix;
    glm::mat4 m_NextTetrisModelMatrix;
    
    glm::mat4x4 m_MapViewMatrix;
    glm::mat4x4 m_NextTetrisViewMatrix;

    uint32_t m_MapArrayID;
    uint32_t m_MapBufferID;
    
    uint32_t m_NextTetrisArrayID;
    uint32_t m_NextTetrisBufferID;
    
    uint32_t m_MapProgramID;
    
    float m_SquareSize = 20.f;

    int32_t m_MapSizeX;
    int32_t m_MapSizeY;
    
    Renderer();

    Vector3f GetColor(const Map& map, int32_t x, int32_t y, int32_t sizeX, int32_t sizeY) const;

    std::vector<float> GenerateFieldData(const Map& map) const;
    std::vector<float> GenerateTetrisData(const Grid& grid, int32_t colorID) const;

    uint32_t CreateShader(uint32_t shaderType, const std::string& shaderSource) const;
    uint32_t CreateProgram(const std::unordered_map<uint32_t, std::string>& sources) const;
    uint32_t LoadShader(const std::string& path) const;

    inline void addPoint(std::vector<float>& data, float x, float y, Vector3f color, float textureX, float textureY) const;
};
