#ifndef COMMAND_H
#define COMMAND_H

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
    bool null_argument_callable;
    std::vector<type> valid_types;
    int max_argument_count;
    std::string help_text;
};
#endif // COMMAND_H
