#include "Tui.h"

#include <chrono>

#include <magic_enum.hpp>

#include "Server.h"
#include "Version.h"

Tui::Tui(/* args */)
{
    windows.server.name = std::string(PROJECT_NAME) + "-" + std::string(VERSION_FULL);
    windows.server.position = {0, 0};
    windows.server.size = {40, 7};

    windows.game.name = "Game";
    windows.game.position = {windows.server.size.x, 0};
    windows.game.size = {60, 30};

    windows.players.name = "Players";
    windows.players.position = {windows.server.size.x + windows.game.size.x, 0};
    windows.players.size = {60, 30};

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    screen = ImTui_ImplNcurses_Init(true);
    ImTui_ImplText_Init();
}

Tui::~Tui()
{
    ImTui_ImplText_Shutdown();
    ImTui_ImplNcurses_Shutdown();

    if (thread_tui.joinable())
        thread_tui.join();
}

void Tui::start()
{
    thread_tui = std::thread(&Tui::loop, this);
}

void Tui::loop()
{
    while (*server->active)
    {
        ImTui_ImplNcurses_NewFrame();
        ImTui_ImplText_NewFrame();

        ImGui::NewFrame();

        // Server window
        ImGui::SetNextWindowPos(windows.server.position, ImGuiCond_Once);
        ImGui::SetNextWindowSize(windows.server.size, ImGuiCond_Once);
        ImGui::Begin(windows.server.name.c_str());
        ImGui::NewLine();
        auto server_uptime = std::chrono::high_resolution_clock::now() - server->timepoint_start;
        ImGui::Text("Server uptime %d days %02d:%02d:%02d", std::chrono::duration_cast<std::chrono::days>(server_uptime), std::chrono::duration_cast<std::chrono::hours>(server_uptime) % 24, std::chrono::duration_cast<std::chrono::minutes>(server_uptime) % 60, std::chrono::duration_cast<std::chrono::seconds>(server_uptime) % 60);
        ImGui::Text("Server tick time %d us", std::chrono::duration_cast<std::chrono::microseconds>(server->tick_duration));
        // ImGui::Text("Float:");
        // ImGui::SameLine();
        // ImGui::SliderFloat("##float", &fval, 0.0f, 10.0f);
        ImGui::NewLine();
        if (ImGui::Button(" Stop server "))
            server->stop();
        ImGui::NewLine();
        ImGui::End();

        // Game window
        ImGui::SetNextWindowPos(windows.game.position, ImGuiCond_Once);
        ImGui::SetNextWindowSize(windows.game.size, ImGuiCond_Once);
        ImGui::Begin(windows.game.name.c_str());
        ImGui::NewLine();
        ImGui::Text("Players: %d/%d", server->players->players.size(), server->preferences->max_players);
        ImGui::Text("Game state: %s", std::string(magic_enum::enum_name(server->game->state)).c_str());
        ImGui::Text("Number of rings: %d", server->game->number_of_rings);
        for (size_t i = 0; i < server->game->rings.size(); ++i)
        {
            auto &ring = server->game->rings[i];
            ImGui::Text("Ring %d Size: %f x: %f z: %f tt: %f", i, ring.radius, ring.center.x, ring.center.z, ring.travelled_time);
        }
        ImGui::NewLine();
        ImGui::End();

        // Players window
        ImGui::SetNextWindowPos(windows.players.position, ImGuiCond_Once);
        ImGui::SetNextWindowSize(windows.players.size, ImGuiCond_Once);
        ImGui::Begin(windows.players.name.c_str());
        ImGui::NewLine();
        for (size_t i = 0; i < server->players->players.size(); ++i)
        {
            auto &player = server->players->players[i];
            std::vector<uint8_t> player_to_teleport;
            player_to_teleport.push_back(player.service.game_index);
            ImGui::Text("Player %d Name: %s", i, player.service.name.c_str());
            ImGui::Text("Position");
            if (ImGui::SliderFloat("X", &player.game.position.x, -1024, 1024))
                server->network->packet_handler.doRequest(ClientEventCode::PlayerRespawn, &player_to_teleport);
            if (ImGui::SliderFloat("Y", &player.game.position.y, -128, 1024))
                server->network->packet_handler.doRequest(ClientEventCode::PlayerRespawn, &player_to_teleport);
            if (ImGui::SliderFloat("Z", &player.game.position.z, -1024, 1024))
                server->network->packet_handler.doRequest(ClientEventCode::PlayerRespawn, &player_to_teleport);
            ImGui::Text("Rotation: x: %f y: %f", player.game.rotation.x, player.game.rotation.y);
            ImGui::Text("Direction: x: %f y: %f z: %f", player.game.direction.x, player.game.direction.y, player.game.direction.z);
        }
        ImGui::NewLine();
        ImGui::End();

        ImGui::Render();

        ImTui_ImplText_RenderDrawData(ImGui::GetDrawData(), screen);
        ImTui_ImplNcurses_DrawScreen();
    }
}