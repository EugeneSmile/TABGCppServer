#include "Interface.h"

#include <string>
#include <functional>

#include <iostream>

#include <magic_enum.hpp>

#include "Server.h"
#include "Version.h"
#include "Config.h"
#include "Logger.h"

Interface::Interface(/* args */)
{
    app.loglevel(crow::LogLevel::WARNING);
    app.signal_clear();

    CROW_ROUTE(app, "/")
    ([]
     {
        crow::mustache::context ctx;
        return crow::mustache::compile("<html><body>"+std::string(PROJECT_NAME)+"-"+std::string(VERSION_FULL)+"</body></html>").render(); });

    CROW_ROUTE(app, "/GetServerList").methods(crow::HTTPMethod::POST)(std::bind(&Interface::getServerList, this, std::placeholders::_1));
    CROW_ROUTE(app, "/GetServerInfo").methods(crow::HTTPMethod::POST)(std::bind(&Interface::getServerInfo, this, std::placeholders::_1));

    CROW_ROUTE(app, "/GetServerData").methods(crow::HTTPMethod::POST)(std::bind(&Interface::getServerData, this, std::placeholders::_1));
    CROW_ROUTE(app, "/SetServerData").methods(crow::HTTPMethod::POST)(std::bind(&Interface::setServerData, this, std::placeholders::_1));

    const std::string interface_address = Config::getValue("address", "0.0.0.0", "Interface");
    const int64_t interface_port = Config::getValue("port", 3010, "Interface");

    app_future = app.bindaddr(interface_address)
                     .port(interface_port)
                     .run_async();

    // app.wait_for_server_start();
    Logger::log->info("Interface started on {}:{}", interface_address, interface_port);
}

Interface::~Interface()
{
    app.stop();
    app_future.wait();
    Logger::log->info("Interface stopped");
}

crow::json::wvalue Interface::getServerList(const crow::request &request)
{
    crow::json::rvalue request_json = crow::json::load(request.body);
    if (request_json)
        std::cout << request_json;

    crow::json::wvalue ret;
    ret["id"] = server->preferences->id;
    ret["serverName"] = server->preferences->name;
    // ret["playersOnServer"] = server->game->players->connected.size();
    ret["splotReserved"] = 0;
    ret["maxPlayers"] = server->preferences->max_players;
    ret["acceptingPlayers"] = server->preferences->accepting_players;
    ret["serverDescription"] = server->preferences->description;
    ret["squadMode"] = server->preferences->squad_mode;
    ret["gameMode"] = server->preferences->game_mode;
    ret["passworded"] = server->preferences->passworded;
    return ret;
}

crow::json::wvalue Interface::getServerInfo(const crow::request &request)
{
    crow::json::wvalue ret;
    crow::json::rvalue request_json = crow::json::load(request.body);
    if (!request_json)
        return ret.empty_object();
    if (request_json["id"] == server->preferences->id)
    {
        if (server->preferences->passworded && request_json["password"] != server->preferences->password)
        {
            ret["error"] = true;
        }
        else
        {
            ret["joinCode"] = server->network->server_address + ":" + std::to_string(server->network->server_port);
            ret["errorCode"] = "null";
        }
    }
    return ret;
}

crow::json::wvalue Interface::getServerData(const crow::request &request)
{
    crow::json::wvalue ret;

    ret["game"]["state"] = static_cast<int64_t>(server->game->state);
    ret["game"]["current_ring_index"] = std::dynamic_pointer_cast<Started>(server->game->phases.at(GameState::Started))->current_ring_index;
    ret["game"]["plane"]["start"]["x"] = std::dynamic_pointer_cast<Flying>(server->game->phases.at(GameState::Flying))->plane.start.x;
    ret["game"]["plane"]["start"]["y"] = std::dynamic_pointer_cast<Flying>(server->game->phases.at(GameState::Flying))->plane.start.y;
    ret["game"]["plane"]["start"]["z"] = std::dynamic_pointer_cast<Flying>(server->game->phases.at(GameState::Flying))->plane.start.z;
    ret["game"]["plane"]["finish"]["x"] = std::dynamic_pointer_cast<Flying>(server->game->phases.at(GameState::Flying))->plane.finish.x;
    ret["game"]["plane"]["finish"]["y"] = std::dynamic_pointer_cast<Flying>(server->game->phases.at(GameState::Flying))->plane.finish.y;
    ret["game"]["plane"]["finish"]["z"] = std::dynamic_pointer_cast<Flying>(server->game->phases.at(GameState::Flying))->plane.finish.z;
    for (size_t i = 0; i < std::dynamic_pointer_cast<Started>(server->game->phases.at(GameState::Started))->number_of_rings; ++i)
    {
        ret["game"]["rings"][i]["radius"] = std::dynamic_pointer_cast<Started>(server->game->phases.at(GameState::Started))->rings[i].size;
        ret["game"]["rings"][i]["travelled_time"] = std::dynamic_pointer_cast<Started>(server->game->phases.at(GameState::Started))->rings[i].travel_time.count();
        ret["game"]["rings"][i]["center"]["x"] = std::dynamic_pointer_cast<Started>(server->game->phases.at(GameState::Started))->rings[i].center.x;
        ret["game"]["rings"][i]["center"]["y"] = std::dynamic_pointer_cast<Started>(server->game->phases.at(GameState::Started))->rings[i].center.y;
        ret["game"]["rings"][i]["center"]["z"] = std::dynamic_pointer_cast<Started>(server->game->phases.at(GameState::Started))->rings[i].center.z;
    }

    for (std::shared_ptr<Player> player : server->game->players->players)
    {
        ret["players"][player->index]["name"] = player->client.name;
        ret["players"][player->index]["group_host"] = player->client.group_host;
        ret["players"][player->index]["group_size"] = player->client.group_size;
        ret["players"][player->index]["auto_squad"] = player->client.auto_squad;
        // ret["players"][player->index]["connected"] = player->client.connected;
        ret["players"][player->index]["dev"] = player->client.dev;
        ret["players"][player->index]["health"] = player->game.health;
        ret["players"][player->index]["position"]["x"] = player->game.position.x;
        ret["players"][player->index]["position"]["y"] = player->game.position.y;
        ret["players"][player->index]["position"]["z"] = player->game.position.z;
        ret["players"][player->index]["rotation"]["x"] = player->game.rotation.x;
        ret["players"][player->index]["rotation"]["y"] = player->game.rotation.y;
        ret["players"][player->index]["direction"]["x"] = player->game.direction.x;
        ret["players"][player->index]["direction"]["y"] = player->game.direction.y;
        ret["players"][player->index]["direction"]["z"] = player->game.direction.z;
        ret["players"][player->index]["aim_down_sight"] = player->game.aim_down_sight;
        ret["players"][player->index]["movement_type"] = static_cast<int64_t>(player->game.movement_type);
        ret["players"][player->index]["flying"] = player->game.flying;
        ret["players"][player->index]["firing_mode"] = static_cast<int64_t>(player->firing.mode);
        ret["players"][player->index]["firing_ammo_type"] = player->firing.ammo_type;
        ret["players"][player->index]["firing_extra_data"] = player->firing.extra_data;
        ret["players"][player->index]["firing_bullet_effect"] = static_cast<int64_t>(player->firing.bullet_effect);
        ret["players"][player->index]["is_driving"] = player->driving.is_driving;
        ret["players"][player->index]["car_id"] = player->driving.car->index;
        ret["players"][player->index]["seat_id"] = player->driving.seat_id;
        ret["players"][player->index]["driving_state"] = static_cast<int64_t>(player->driving.state);
    }
    return ret;
}

crow::response Interface::setServerData(const crow::request &request)
{
    auto req = crow::json::load(request.body);
    if (!req)
        return crow::response(400);

    for (auto &key : req.keys())
    {
        if (key == "game_state")
            server->game->changeState(static_cast<GameState>(req[key].i()));
    }
    return crow::response(200);
}
