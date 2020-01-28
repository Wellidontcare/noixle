#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <algorithm>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "command.h"

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
