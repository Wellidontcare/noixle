#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H
#include <unordered_map>
#include <sstream>
#include <algorithm>
#include <string>
#include <vector>

enum TYPE{
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
    std::vector<TYPE> valid_types;
    int num_arguments;
    std::string help_text;
};

class CommandParser
{
std::vector<Command> available_commands_;
public:
    CommandParser(const std::vector<Command>& available_commands);
    Command parse(const char* input);

private:
    bool has_correct_types(Command command, std::vector<Arg>& args);
};

#endif // COMMANDPARSER_H
