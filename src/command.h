#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <vector>

/**
 * Container for message parameters
 */
struct CommandParam {
    int requestType;
    int numericValue = 0;
    std::string textValue = "";
};

class Command {
public:
    Command(int command, std::vector<CommandParam> parameters);

    int getCommand();
    std::vector<CommandParam> getParameters();

private:
    int command;
    std::vector<CommandParam> parameters;
};


#endif
