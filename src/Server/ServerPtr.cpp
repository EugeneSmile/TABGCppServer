#include "ServerPtr.h"

void ServerPtr::init(std::shared_ptr<Server> server)
{
    ServerPtr::server = server;
}