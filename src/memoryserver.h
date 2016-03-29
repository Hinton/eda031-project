#ifndef MEMORYSERVER_H
#define MEMORYSERVER_H

#include <memory>
#include "server.h"
#include "connection.h"
#include "messagehandler.h"
#include "idatabase.h"

class MemoryServer {
public:
    MemoryServer(int port, IDatabase *database);

    void run();
private:
    IDatabase *database;
    Server server;
    MessageHandler parser;
};


#endif
