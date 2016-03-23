#ifndef MEMORYSERVER_H
#define MEMORYSERVER_H

#include <memory>
#include "server.h"
#include "connection.h"
#include "messagehandler.h"

class MemoryServer {
public:
    MemoryServer(int port);

    void run();
private:
    Server server;
    MessageHandler parser;
};


#endif
