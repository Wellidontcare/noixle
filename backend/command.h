#ifndef COMMAND_H
#define COMMAND_H
#endif // COMMAND_H

#include <string>
#include <vector>

enum type{
    INT,
    FLOAT,
    STRING
};

struct Arg{
    int int_arg;
    double float_arg;
    std::string string_arg;
};

struct Command{
    std::string command;
    std::vector<Arg> args;
    bool no_arguments_allowed;
    std::vector<type> valid_types;
    int num_arguments;
    std::string help_text;
};
