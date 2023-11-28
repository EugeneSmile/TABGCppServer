#include "Interface.h"

#include <string>
#include <functional>

#include <iostream>

#include "Server.h"
#include "Version.h"
#include "Config.h"
#include "Logger.h"

Interface::Interface(/* args */)
{
    // app.loglevel(crow::LogLevel::WARNING);
    app.signal_clear();

    CROW_ROUTE(app, "/")
    ([]
     {
        crow::mustache::context ctx;
        return crow::mustache::compile("<html><body>"+std::string(PROJECT_NAME)+"-"+std::string(VERSION_FULL)+"</body></html>").render(); });

    CROW_ROUTE(app, "/GetServerList").methods(crow::HTTPMethod::POST)(std::bind(&Interface::getServerList, this, std::placeholders::_1));
    CROW_ROUTE(app, "/GetServerInfo").methods(crow::HTTPMethod::POST)(std::bind(&Interface::getServerInfo, this, std::placeholders::_1));

    const std::string interface_address = Config::getValue("address", "0.0.0.0", "Interface");
    const int64_t interface_port = Config::getValue("port", 3010, "Interface");

    app_future = app.bindaddr(interface_address)
                     .port(interface_port)
                     .run_async();

    app.wait_for_server_start();
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
    ret["playersOnServer"] = server->players->getPlayersCount();
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