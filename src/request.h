#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <vector>

/**
 * Container for message parameters
 */
struct RequestParam {
    int requestType;
    int numericValue = 0;
    std::string textValue = "";
};

class Request {
public:
    Request(int command, std::vector<RequestParam> parameters);

    int getCommand();
    std::vector<RequestParam> getParameters();

private:
    int command;
    std::vector<RequestParam> parameters;
};


#endif
