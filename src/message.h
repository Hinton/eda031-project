#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <vector>

/**
 * Container for message parameters
 */
struct MessageParameter {
    int commandType;
    int numericValue = 0;
    std::string textValue = "";
};

class Message {
public:
    Message(int command, std::vector<MessageParameter> parameters);

    int getCommand();
    std::vector<MessageParameter> getParameters();

private:
    int command;
    std::vector<MessageParameter> parameters;
};


#endif
