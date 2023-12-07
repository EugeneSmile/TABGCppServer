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
    ret["playersOnServer"] = server->players->connected.size();
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
    ret["game"]["current_ring_index"] = server->game->current_ring_index;
    ret["game"]["plane"]["start"]["x"] = server->game->plane.start.x;
    ret["game"]["plane"]["start"]["y"] = server->game->plane.start.y;
    ret["game"]["plane"]["start"]["z"] = server->game->plane.start.z;
    ret["game"]["plane"]["finish"]["x"] = server->game->plane.finish.x;
    ret["game"]["plane"]["finish"]["y"] = server->game->plane.finish.y;
    ret["game"]["plane"]["finish"]["z"] = server->game->plane.finish.z;
    for (size_t i = 0; i < server->game->number_of_rings; ++i)
    {
        ret["game"]["rings"][i]["radius"] = server->game->rings[i].radius;
        ret["game"]["rings"][i]["travelled_time"] = server->game->rings[i].travelled_time;
        ret["game"]["rings"][i]["center"]["x"] = server->game->rings[i].center.x;
        ret["game"]["rings"][i]["center"]["y"] = server->game->rings[i].center.y;
        ret["game"]["rings"][i]["center"]["z"] = server->game->rings[i].center.z;
    }

    for (auto player : server->players->players)
    {
        ret["players"][player.service.game_index]["name"] = player.service.name;
        ret["players"][player.service.game_index]["group_index"] = player.service.group_index;
        ret["players"][player.service.game_index]["squad_host"] = player.service.squad_host;
        ret["players"][player.service.game_index]["squad_members"] = player.service.squad_members;
        ret["players"][player.service.game_index]["auto_squad"] = player.service.auto_squad;
        ret["players"][player.service.game_index]["connected"] = player.service.connected;
        ret["players"][player.service.game_index]["dev"] = player.service.dev;
        ret["players"][player.service.game_index]["health"] = player.game.health;
        ret["players"][player.service.game_index]["position"]["x"] = player.game.position.x;
        ret["players"][player.service.game_index]["position"]["y"] = player.game.position.y;
        ret["players"][player.service.game_index]["position"]["z"] = player.game.position.z;
        ret["players"][player.service.game_index]["rotation"]["x"] = player.game.rotation.x;
        ret["players"][player.service.game_index]["rotation"]["y"] = player.game.rotation.y;
        ret["players"][player.service.game_index]["direction"]["x"] = player.game.direction.x;
        ret["players"][player.service.game_index]["direction"]["y"] = player.game.direction.y;
        ret["players"][player.service.game_index]["direction"]["z"] = player.game.direction.z;
        ret["players"][player.service.game_index]["aim_down_sight"] = player.game.aim_down_sight;
        ret["players"][player.service.game_index]["movement_type"] = static_cast<int64_t>(player.game.movement_type);
        ret["players"][player.service.game_index]["flying"] = player.game.flying;
        ret["players"][player.service.game_index]["firing_mode"] = static_cast<int64_t>(player.game.firing.mode);
        ret["players"][player.service.game_index]["firing_ammo_type"] = player.game.firing.ammo_type;
        ret["players"][player.service.game_index]["firing_extra_data"] = player.game.firing.extra_data;
        ret["players"][player.service.game_index]["firing_bullet_effect"] = static_cast<int64_t>(player.game.firing.bullet_effect);
        ret["players"][player.service.game_index]["is_driving"] = player.game.driving.is_driving;
        ret["players"][player.service.game_index]["car_id"] = player.game.driving.car_id;
        ret["players"][player.service.game_index]["seat_id"] = player.game.driving.seat_id;
        ret["players"][player.service.game_index]["driving_state"] = static_cast<int64_t>(player.game.driving.state);
    }
    return ret;
}
