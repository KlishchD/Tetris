#pragma once
#include "GlobalDefines.h"
#include <cstdint>


class Window
{
public:
    Window(int32_t sizeX, int32_t sizeY);

    bool ShouldBeOpen() const;
    void Clear();
    void SwapBuffers();
    void PollEvents();

    ~Window();
private:
    GLFWwindow* m_Window;
};
