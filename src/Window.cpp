#include "Window.h"

#include "Game.h"

void OnKeyboardAction(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_RIGHT: Game::Get().Move(-1); break;
        case GLFW_KEY_E:     Game::Get().Rotate(1); break;

        case GLFW_KEY_LEFT:  Game::Get().Move(1); break;
        case GLFW_KEY_Q:     Game::Get().Rotate(-1); break;
        }
    }
}

Window::Window(int32_t sizeX, int32_t sizeY)
{
    if (!glfwInit())
    {
        return;
    }
    
    m_Window = glfwCreateWindow(sizeX, sizeY, "Hello World", NULL, NULL);

    if (!m_Window)
    {
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_Window);

    if (glewInit() != GLEW_OK)
    {
        return;
    }

    glfwSetKeyCallback(m_Window, OnKeyboardAction);
}

bool Window::ShouldBeOpen() const
{
    return !glfwWindowShouldClose(m_Window);
}

void Window::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::SwapBuffers()
{
    glfwSwapBuffers(m_Window);
}

void Window::PollEvents()
{
    glfwPollEvents();
}

Window::~Window()
{
    glfwTerminate();
}