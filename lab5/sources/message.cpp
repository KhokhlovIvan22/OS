#include "message.h"
#include <cstring>

using std::strncpy;
using std::strncmp;

void Message::setCommand(const char* cmd) {
    strncpy(command, cmd, sizeof(command) - 1);
    command[sizeof(command) - 1] = '\0';
}

bool Message::isCommand(const char* cmd) const {
    return strncmp(command, cmd, sizeof(command)) == 0;
}