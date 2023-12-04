#ifndef NETWORK_INTERFACE_H_GUARD
#define NETWORK_INTERFACE_H_GUARD

#include <crow.h>
#include <crow/middlewares/cors.h>

#include "ServerPtr.h"

class Interface : public ServerPtr
{
private:
    crow::json::wvalue getServerList(const crow::request &request);
    crow::json::wvalue getServerInfo(const crow::request &request);
    crow::json::wvalue console(const crow::request &request);
    crow::App<crow::CORSHandler> app;
    std::future<void> app_future;

public:
    Interface(/* args */);
    ~Interface();
};

#endif