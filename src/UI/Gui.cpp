#include "Gui.h"

#include <magic_enum.hpp>
#include <imgui-SFML.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Server.h"

Gui::Gui(/* args */)
{
    windows.server.name = std::string(PROJECT_NAME) + "-" + std::string(VERSION_FULL);
    windows.server.position = {0, 0};
    windows.server.size = {300, 150};

    windows.game.name = "Game";
    windows.game.position = {windows.server.size.x, 0};
    windows.game.size = {400, 400};

    windows.players.name = "Players";
    windows.players.position = {windows.server.size.x + windows.game.size.x, 0};
    windows.players.size = {400, 600};
}

Gui::~Gui()
{
    if (thread_main_window.joinable())
        thread_main_window.join();
}

void Gui::start()
{
    thread_main_window = std::thread(&Gui::threadLoop, this);
}

void Gui::threadLoop()
{
    sf::RenderWindow window(sf::VideoMode(1280, 800), "ImGui + SFML = <3");
    window.setFramerateLimit(60);
    if (!ImGui::SFML::Init(window))
        return;

    sf::Clock deltaClock;

    while (*server->active)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        // Server window
        ImGui::SetNextWindowPos(windows.server.position, ImGuiCond_Once);
        ImGui::SetNextWindowSize(windows.server.size, ImGuiCond_Once);
        ImGui::Begin(windows.server.name.c_str());
        ImGui::NewLine();
        auto server_uptime = std::chrono::high_resolution_clock::now() - server->timepoint_start;
        ImGui::Text("Server uptime %ld days %02ld:%02ld:%02ld", std::chrono::duration_cast<std::chrono::days>(server_uptime).count(), std::chrono::duration_cast<std::chrono::hours>(server_uptime).count() % 24, std::chrono::duration_cast<std::chrono::minutes>(server_uptime).count() % 60, std::chrono::duration_cast<std::chrono::seconds>(server_uptime).count() % 60);
        ImGui::Text("Server tick time %ld us", std::chrono::duration_cast<std::chrono::microseconds>(server->tick_duration).count());
        // ImGui::Text("Float:");
        // ImGui::SameLine();
        // ImGui::SliderFloat("##float", &fval, 0.0f, 10.0f);
        ImGui::NewLine();
        if (ImGui::Button(" Stop server "))
            server->stop();
        ImGui::SameLine();
        if (ImGui::Button(" Randomize plane "))
        {
            std::dynamic_pointer_cast<Flying>(server->game->phases.at(GameState::Flying))->randomizePlane();
            server->game->changeState(GameState::Flying, true);
        }
        ImGui::NewLine();
        ImGui::End();

        // Game window
        ImGui::SetNextWindowPos(windows.game.position, ImGuiCond_Once);
        ImGui::SetNextWindowSize(windows.game.size, ImGuiCond_Once);
        ImGui::Begin(windows.game.name.c_str());
        ImGui::NewLine();
        ImGui::Text("Players: %ld/%d", server->players->players.size(), server->preferences->max_players);
        ImGui::BeginTable("Game states", 1);
        ImGui::TableSetupColumn("Name");
        ImGui::TableHeadersRow();
        for (size_t i = 0; i < magic_enum::enum_count<GameState>(); ++i)
        {
            ImGui::TableNextRow();
            std::string text = std::string(magic_enum::enum_names<GameState>().at(i));
            ImGui::Text(text.c_str());
        }
        ImGui::EndTable();
        ImGui::Text("Game state: %s", std::string(magic_enum::enum_name(server->game->state)).c_str());
        ImGui::Text("Number of rings: %d", std::dynamic_pointer_cast<Started>(server->game->phases.at(GameState::Started))->number_of_rings);
        for (size_t i = 0; i < std::dynamic_pointer_cast<Started>(server->game->phases.at(GameState::Started))->rings.size(); ++i)
        {
            auto &ring = std::dynamic_pointer_cast<Started>(server->game->phases.at(GameState::Started))->rings.at(i);
            ImGui::Text("Ring %ld Size: %f x: %f z: %f", i, ring.size, ring.center.x, ring.center.z);
            float ring_tt = ring.travel_time.count();
            if (ImGui::SliderFloat("Travel time", &ring_tt, 0, 15))
                ring.travel_time = std::chrono::duration<float>(ring_tt);
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
            ImGui::Text("Player %ld Name: %s", i, player.service.name.c_str());
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
        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }
    if (window.isOpen())
        window.close();
    ImGui::SFML::Shutdown();
}