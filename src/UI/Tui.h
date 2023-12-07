#ifndef UI_TUI_H_GUARD
#define UI_TUI_H_GUARD

#include <thread>

#include <imtui/imtui.h>
#include <imtui/imtui-impl-ncurses.h>

#include "ServerPtr.h"

struct WindowData
{
    std::string name;
    ImVec2 position;
    ImVec2 size;
};

class Tui : public ServerPtr
{
private:
    struct Windows
    {
        WindowData server;
        WindowData game;
        WindowData players;
    } windows;

    int nframes = 0;
    float fval = 1.23f;
    std::thread thread_tui;
    ImTui::TScreen *screen;
    void loop();

public:
    Tui(/* args */);
    ~Tui();
    void start();
};

#endif