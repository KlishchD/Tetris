#pragma once
#include <memory>

#include "Map.h"
#include "Renderer.h"
#include "TetrisCollection.h"
#include "Vector.h"
#include "Window.h"

class Game
{
public:
    static void CreateGame(Vector2i mapSize);
    static void DeleteGame();
    static Game& Get();

    void Start();

    void Update(float DeltaTime);

    void Move(int32_t move);
    void Rotate(int32_t rotation);
private:
    std::unique_ptr<Window> m_Window;
    
    const TetrisCollection& m_TetrisCollection = TetrisCollection::Get();
    const ColorLibrary& m_ColorLibrary = ColorLibrary::Get();

    Tetris m_NextTetris;
    int32_t m_NextTetrisColorId;
    
    Tetris m_Tetris;
    int32_t m_TetrisColorId;

    int32_t m_Rotation;
    Vector2i m_Position;

    bool m_IsAlive = true;

    Map m_Map;

    Game(Vector2i mapSize);
    
    void ResetState()
    {
        m_Tetris = m_TetrisCollection.GetRandomTetris();
        m_TetrisColorId = m_ColorLibrary.GetRandomColorId();
        m_Rotation = 0;
        m_Position = {5, 0};
    }
};
