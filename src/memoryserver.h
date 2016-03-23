#ifndef MEMORYSERVER_H
#define MEMORYSERVER_H

#include "server.h"
#include "connection.h"

class MemoryServer {
public:
    MemoryServer(int port);

    void run();
private:
    Server server;

};


#endif
