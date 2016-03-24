#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <vector>

/**
 * Container for message parameters
 */
struct ResponseParam {
    int requestType;
    int numericValue = 0;
    std::string textValue = "";
};

class Response {
public:
    Response(int command, std::vector<ResponseParam> parameters);

    int getCommand();
    std::vector<ResponseParam> getParameters();

private:
    int command;
    std::vector<ResponseParam> parameters;
};


#endif
