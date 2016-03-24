#include <vector>
#include "request.h"

using namespace std;

Request::Request(int command, vector <RequestParam> parameters) : command(command), parameters(parameters) {

}

int Request::getCommand() {
    return command;
}

std::vector<RequestParam> Request::getParameters() {
    return parameters;
}


