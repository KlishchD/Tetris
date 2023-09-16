#include "Game.h"
#include "GlobalDefines.h"
#include <iostream>
#include <thread>


using namespace std::chrono_literals;

Game* game = nullptr;

Game::Game(Vector2i mapSize): m_Map(mapSize.x, mapSize.y)
{
}

void Game::CreateGame(Vector2i mapSize)
{
    DeleteGame();
    game = new Game(mapSize);
}

void Game::DeleteGame()
{
    if (game)
    {
        delete game;
        game = nullptr;
    }
}

Game& Game::Get()
{
    return *game;
}

void Game::Start()
{
    ResetState();
    m_Map.Reset();

    m_NextTetris = m_TetrisCollection.GetRandomTetris();
    m_NextTetrisColorId = m_ColorLibrary.GetRandomColorId();
    
    m_Window = std::make_unique<Window>(640, 480);

    Renderer::Get().SetUp(640.0f, 480.0f, m_Map.GetSizeX(), m_Map.GetSizeY());
    
    while (m_Window->ShouldBeOpen())
    {
        m_Window->Clear();

        Update(0.1f);

        m_Window->SwapBuffers();
        m_Window->PollEvents();
    }
}

void Game::Update(float DeltaTime)
{
    if (m_IsAlive)
    {
        m_Map.RemoveTetrisFromRendering(m_Position, m_Tetris, m_Rotation);

        if (m_Map.WillCollide(m_Position, m_Tetris, m_Rotation))
        {
            m_Map.AddTetris(m_Position, m_Tetris, m_Rotation, m_TetrisColorId);

            m_Tetris = m_NextTetris;
            m_TetrisColorId = m_NextTetrisColorId;

            m_NextTetris = m_TetrisCollection.GetRandomTetris();
            m_NextTetrisColorId = m_ColorLibrary.GetRandomColorId();

            m_Rotation = 0;
            m_Position = {5, 0};
            
            int32_t Lines = m_Map.DoLineCheck();

            if (Lines)
            {
                while (Lines--)
                {
                    m_Map.DropLine();

                    m_Window->SwapBuffers();
                
                    Renderer::Get().DrawField(m_Map);
    
                    m_Window->SwapBuffers();

                    std::this_thread::sleep_for(400ms);
                }

                m_Window->Clear();
            }
            
            m_IsAlive = !m_Map.WillCollide(m_Position, m_Tetris, m_Rotation);
        }
        else
        {
            m_Position.y++;     
        }

        if (m_IsAlive)
        {
            m_Map.AddTetrisForRendering(m_Position, m_Tetris, m_Rotation, m_TetrisColorId);
        
            std::cout << "Next iteration" << std::endl;
        }
        else
        {
            // draw lose status
            std::cout << "You lost it ;)" << std::endl;
        }
    }

    Renderer::Get().DrawField(m_Map);
    Renderer::Get().DrawNextTetris(m_NextTetris, m_NextTetrisColorId);
    
    std::this_thread::sleep_for(100ms);
}

void Game::Move(int32_t move)
{
    if (m_IsAlive && m_Map.CanMoveTo({m_Position.x + move, m_Position.y}, m_Tetris, m_Rotation))
    {
        m_Map.RemoveTetrisFromRendering(m_Position, m_Tetris, m_Rotation);
        m_Position.x += move;
        m_Map.AddTetrisForRendering(m_Position, m_Tetris, m_Rotation, m_TetrisColorId);
    }
}

void Game::Rotate(int32_t rotation)
{
    if (m_IsAlive && m_Map.CanMoveTo(m_Position, m_Tetris, m_Rotation + rotation)) {
        m_Map.RemoveTetrisFromRendering(m_Position, m_Tetris, m_Rotation);
        m_Rotation += rotation;
        m_Map.AddTetrisForRendering(m_Position, m_Tetris, m_Rotation, m_TetrisColorId);
    }
}
