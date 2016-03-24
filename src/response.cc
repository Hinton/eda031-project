#include <vector>
#include "response.h"

using namespace std;

Response::Response(int command, vector <ResponseParam> parameters) : command(command), parameters(parameters) {

}

int Response::getCommand() {
    return command;
}

std::vector<ResponseParam> Response::getParameters() {
    return parameters;
}


