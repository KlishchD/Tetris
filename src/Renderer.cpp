#include "Renderer.h"
#include <fstream>
#include <iostream>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Map.h"

Renderer& Renderer::Get()
{
    static Renderer renderer;
    return renderer;
}

void Renderer::SetUp(float WindowSizeX, float WindowSizeY, int32_t mapSizeX, int32_t mapSizeY)
{
    m_MapSizeX = mapSizeX;
    m_MapSizeY = mapSizeY;
    
    m_MapModelMatrix = glm::translate(glm::mat4(1), glm::vec3(-m_SquareSize * (mapSizeX / 2), 0.0f, 0.0f));
    
    m_MapViewMatrix = glm::ortho(-WindowSizeX / 2.0f, WindowSizeX / 2.0f, 0.0f, WindowSizeY, -1.0f, 1.0f);
    m_NextTetrisViewMatrix = glm::ortho(-WindowSizeX / 2.0f, WindowSizeX / 2.0f, 0.0f, WindowSizeY, -1.0f, 1.0f);

    glGenVertexArrays(1, &m_MapArrayID);
    glBindVertexArray(m_MapArrayID);
    
    glGenBuffers(1, &m_MapBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_MapBufferID);

    float stride = 7 * sizeof(float);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));

    glGenVertexArrays(1, &m_NextTetrisArrayID);
    glBindVertexArray(m_NextTetrisArrayID);
    
    glGenBuffers(1, &m_NextTetrisBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_NextTetrisBufferID);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, nullptr);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
    
    m_MapProgramID = LoadShader("C:\\Users\\Dklishch\\RiderProjects\\Tetris\\shaders\\grid.glsl");
        
    glUseProgram(m_MapProgramID);
}

void Renderer::DrawField(const Map& map)
{
    glBindVertexArray(m_MapArrayID);
    glBindBuffer(GL_ARRAY_BUFFER, m_MapBufferID);
    
    std::vector<float> data = GenerateFieldData(map);

    glUniformMatrix4fv(glGetUniformLocation(m_MapProgramID, "u_ViewProjectionScreenMatrix"), 1, GL_FALSE, glm::value_ptr(m_MapViewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(m_MapProgramID, "u_ModelMatrix"), 1, GL_FALSE, glm::value_ptr(m_MapModelMatrix));

    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
    glDrawArrays(GL_QUADS, 0, data.size() / 5);
}

void Renderer::DrawNextTetris(const Tetris& tetris, int32_t colorId)
{
    m_NextTetrisModelMatrix = glm::translate(glm::mat4(1), glm::vec3(m_SquareSize * m_MapSizeX, m_SquareSize * (m_MapSizeY - tetris.GetGrid(0).size()) / 2, 0.0f));

    glBindVertexArray(m_NextTetrisArrayID);
    glBindBuffer(GL_ARRAY_BUFFER, m_NextTetrisBufferID);

    std::vector<float> data = GenerateTetrisData(tetris.GetGrid(0), colorId);

    glUniformMatrix4fv(glGetUniformLocation(m_MapProgramID, "u_ViewProjectionScreenMatrix"), 1, GL_FALSE, glm::value_ptr(m_NextTetrisViewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(m_MapProgramID, "u_ModelMatrix"), 1, GL_FALSE, glm::value_ptr(m_NextTetrisModelMatrix));

    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
    glDrawArrays(GL_QUADS, 0, data.size() / 5);
}

Renderer::Renderer()
{
   
}

Vector3f Renderer::GetColor(const Map& map, int32_t x, int32_t y, int32_t sizeX, int32_t sizeY) const
{
    x = std::max(0, std::min(sizeX - 1, x));
    y = std::max(0, std::min(sizeY - 1, y));
        
    return m_ColorLibrary.GetColor(map.GetMap()[y][x].RenderColorId);
}

std::vector<float> Renderer::GenerateFieldData(const Map& map) const
{
    std::vector<float> result;
        
    int32_t sizeX = map.GetSizeX();
    int32_t sizeY = map.GetSizeY();

    result.reserve(sizeX * sizeY * 35);
        
    for (float y = 0; y < map.GetSizeY(); ++y)
    {
        for (float x = 0; x < map.GetSizeX(); ++x)
        {
            Vector3f Color = GetColor(map, x, y, sizeX, sizeY);

            addPoint(result, (sizeX - x)        * m_SquareSize, (sizeY - y)        * m_SquareSize, Color, 1.0f, 1.0f);
            addPoint(result, (sizeX - x - 1.0f) * m_SquareSize, (sizeY - y)        * m_SquareSize, Color, 0.0f, 1.0f);
            addPoint(result, (sizeX - x - 1.0f) * m_SquareSize, (sizeY - y - 1.0f) * m_SquareSize, Color, 0.0f, 0.0f);
            addPoint(result, (sizeX - x)        * m_SquareSize, (sizeY - y - 1.0f) * m_SquareSize, Color, 1.0f, 0.0f);
        }
    }

    return result;
}

std::vector<float> Renderer::GenerateTetrisData(const Grid& grid, int32_t colorId) const
{
    std::vector<float> data;

    int32_t sizeY = grid.size();
    int32_t sizeX = grid[0].size();
    
    for (int32_t y = 0; y < grid.size(); ++y)
    {
        for (int32_t x = 0; x < grid[y].size(); ++x)
        {
            Vector3f color;

            if (grid[y][x])
            {
                color = m_ColorLibrary.GetColor(colorId);
            }
            else
            {
                color = m_ColorLibrary.GetColor(1);
            }

            addPoint(data, (sizeX - x)     * m_SquareSize, (sizeY - y)     * m_SquareSize, color, 0.0f, 0.0f);
            addPoint(data, (sizeX - x)     * m_SquareSize, (sizeY - y - 1) * m_SquareSize, color, 0.0f, 1.0f);
            addPoint(data, (sizeX - x - 1) * m_SquareSize, (sizeY - y - 1) * m_SquareSize, color, 1.0f, 1.0f);
            addPoint(data, (sizeX - x - 1) * m_SquareSize, (sizeY - y)     * m_SquareSize, color, 1.0f, 0.0f);
        }
    }
    
    return data;
}

uint32_t Renderer::CreateShader(uint32_t shaderType, const std::string& shaderSource) const
{
    const uint32_t shaderId = glCreateShader(shaderType);

    const char* shaderSourceC = shaderSource.data();
    glShaderSource(shaderId, 1, &shaderSourceC, 0);

    glCompileShader(shaderId);

    int32_t status;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        char message[1024];
        int32_t messageLength;
        glGetShaderInfoLog(shaderId, 1024, &messageLength, message);
        std::cout << shaderType << "::" << message;
    }

    return shaderId;
}

uint32_t Renderer::CreateProgram(const std::unordered_map<uint32_t, std::string>& sources) const
{
    int32_t id = glCreateProgram();

    std::vector<int32_t> shadersIds;
        
    for (const auto& entry : sources) {
        glAttachShader(id, CreateShader(entry.first, entry.second));
    }

    glLinkProgram(id);
    glValidateProgram(id);

    for (const auto& shaderId : shadersIds) {
        glDetachShader(id, shaderId);
        glDeleteShader(shaderId);
    }
        
    return id;
}

uint32_t Renderer::LoadShader(const std::string& path) const
{
    std::unordered_map<uint32_t, std::string> sources;

    std::string source;
    uint32_t currentShaderType = UINT32_MAX;

    std::fstream file(path, std::ios_base::in);
    std::string line;

    while (std::getline(file, line)) {
        if (line.find("// type") != std::string::npos) {
            if (currentShaderType != UINT32_MAX) {
                sources[currentShaderType] = source;
            } 
            source.clear();
			     
            if (line.find("fragment") != std::string::npos) {
                currentShaderType = GL_FRAGMENT_SHADER;
            } else if (line.find("vertex") != std::string::npos) {
                currentShaderType = GL_VERTEX_SHADER;
            } else {
                std::cout << "ERROR: NOT A SUPPORTED SHADER TYPE" << std::endl;
                currentShaderType = UINT32_MAX;
            }
        } else {
            source += line + "\n";
        }
    }

    if (currentShaderType != UINT32_MAX) {
        sources[currentShaderType] = source;
    }
	    
    return CreateProgram(sources);
}

void Renderer::addPoint(std::vector<float>& data, float x, float y, Vector3f color, float textureX, float textureY) const
{
    data.push_back(x);
    data.push_back(y);

    data.push_back(color.x);
    data.push_back(color.y);
    data.push_back(color.z);

    data.push_back(textureX);
    data.push_back(textureY);
}
