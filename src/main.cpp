#define GLEW_STATIC

#include <fstream>

#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
#include "Map.h"
#include "Vector.h"
#include <chrono>

using namespace std::chrono_literals;

TetrisCollection& tetrisCollection = TetrisCollection::Get();

Vector2i position = { 5, 0 };
Tetris tetris = tetrisCollection.GetRandomTetris();

int32_t rotation = 0;

bool isAlive = true;

Map map(10, 20);

uint32_t CreateShader(uint32_t shaderType, const std::string& shaderSource) {
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

uint32_t CreateProgram(const std::unordered_map<uint32_t, std::string>& sources)
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

int32_t loadShader(const std::string& path)
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


void OnKeyboardAction(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_RIGHT)
        {
            if (map.CanMoveFromTo(position, {position.x - 1, position.y}, tetris, rotation))
            {
                map.RemoveTetris(position, tetris, rotation);
                position.x--;
                map.AddTetris(position, tetris, rotation);
            }
        } else if (key == GLFW_KEY_LEFT)
        {
            if (map.CanMoveFromTo(position, {position.x + 1, position.y}, tetris, rotation))
            {
                map.RemoveTetris(position, tetris, rotation);
                position.x++;
                map.AddTetris(position, tetris, rotation);
            }
        }

        if (key == GLFW_KEY_Q)
        {
            map.RemoveTetris(position, tetris, rotation);
            rotation--;
            map.AddTetris(position, tetris, rotation);
        }
        if (key == GLFW_KEY_E)
        {
            map.RemoveTetris(position, tetris, rotation);
            rotation++;
            map.AddTetris(position, tetris, rotation);
        }
    }
}

int main(int argc, char* argv[])
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;
    
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        return -1;
    }

   

    /*std::vector<Vector2f> points = map.GeneratePoints({0.05f, 0.05f});
    std::vector<Vector3f> colors = map.GenerateColors();
    std::vector<int32_t> indices = map.GenerateIndexes();*/

    std::vector<float> data = map.GenerateData({0.05f, 0.05});

    uint32_t dataBuffer;
    glGenBuffers(1, &dataBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, dataBuffer);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (GLvoid*)(2 * sizeof(float)));

    /*
    uint32_t pointsBufferID;
    glGenBuffers(1, &pointsBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, pointsBufferID);
    glBufferData(GL_ARRAY_BUFFER, points.size() * 2 * sizeof(float), points.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_TRUE, 0, 0);
    
    uint32_t colorsBufferID;
    glGenBuffers(1, &colorsBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, colorsBufferID);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * 3 * sizeof(float), colors.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    uint32_t indicesBufferId;
    glGenBuffers(1, &indicesBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int32_t), indices.data(), GL_STATIC_DRAW);
    */

    int32_t programID = loadShader("C:\\Users\\Dklishch\\RiderProjects\\Tetris\\shaders\\grid.glsl");

    glUseProgram(programID);

    glfwSetKeyCallback(window, OnKeyboardAction);
    
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        /*colors = map.GenerateColors();
        glBindBuffer(GL_ARRAY_BUFFER, colorsBufferID);
        glBufferData(GL_ARRAY_BUFFER, colors.size() * 3 * sizeof(float), colors.data(), GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBufferId);
        glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_INT, 0);

        for (int32_t i = 0; i < indices.size(); ++i)
        {
            int32_t index = indices[i];
            std::cout << points[index].x * 20 << " " << points[index].y * 20 << std::endl;
            if (i % 4 == 3)
            {
//                std::cout << "#############" << std::endl;
            }
        }
*/

        data = map.GenerateData({0.05f, 0.05f});
        
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
        glDrawArrays(GL_QUADS, 0, data.size() / 5);

        
        if (isAlive)
        {
            map.RemoveTetris(position, tetris, rotation);

            if (map.WillCollide(position, tetris, rotation))
            {
                isAlive = map.IsAlive(position, tetris, rotation);
            
                if (isAlive)
                {
                    map.AddTetris(position, tetris, rotation);
                    position = {5, 0};
                    tetris = tetrisCollection.GetRandomTetris();
                    rotation = 0;
                }
                else
                {
                    // draw lose status
                    std::cout << "You lost it ;)" << std::endl;
                }
            }
            else
            {
                position.y++;     
            }

            map.AddTetris(position, tetris, rotation);
        
            std::cout << "Next iteration" << std::endl;

            for (const std::vector<GridFieldStatus>& row: map.GetMap())
            {
                for (const GridFieldStatus& item: row)
                {
                //    std::cout << (item == GFS_Taken ? 1 : 0);
                }
                //std::cout << std::endl;
            }
        }

        std::this_thread::sleep_for(100ms);
        
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    
    return 0;
}
