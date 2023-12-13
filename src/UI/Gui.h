#ifndef UI_GUI_H_GUARD
#define UI_GUI_H_GUARD

#include <thread>

#include <imgui.h>

#include "ServerPtr.h"

class Gui : public ServerPtr
{
private:
    struct WindowData
    {
        std::string name;
        ImVec2 position;
        ImVec2 size;
    };
    struct Windows
    {
        WindowData server;
        WindowData game;
        WindowData players;
    } windows;

    std::thread thread_main_window;
    void threadLoop();

public:
    Gui(/* args */);
    ~Gui();
    void start();
};

#endif