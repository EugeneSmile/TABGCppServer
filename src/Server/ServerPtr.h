#ifndef SERVER_SERVERPTR_H_GUARD
#define SERVER_SERVERPTR_H_GUARD

#include <memory>

class Server;
class ServerPtr
{
protected:
    inline static std::shared_ptr<Server> server;

public:
    static void init(std::shared_ptr<Server> server);
};

#endif