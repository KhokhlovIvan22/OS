#pragma once
#include "employee.h"

struct Message {
    char command[16];
    int id; // employee ID
    employee emp; //relevant employee

    void setCommand(const char* cmd);
    bool isCommand(const char* cmd) const;
};
