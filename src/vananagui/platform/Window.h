#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <glm/ext/vector_int2.hpp>

class Window {
private:
    HWND hwnd;
    HDC hdc;
    int width;
    int height;
public:
    Window(void* hwnd, const char* title, glm::ivec2 dimensions);
    ~Window();
    void show(bool show = true);
    HWND getHwnd();
    HDC getHdc();
    int getWidth();
    int getHeight();
};
