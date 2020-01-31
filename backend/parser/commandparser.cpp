#include "commandparser.h"

CommandParser::CommandParser(const std::vector<Command>& available_commands)
    : available_commands_(available_commands) {}

Command CommandParser::parse(const char *input)
{
    std::stringstream input_stream(input);
    std::string command;
    std::vector<Arg> args;
    input_stream >> command;
    std::string arg;
    while(input_stream >> arg){
        args.emplace_back(Arg{0, 0, arg});
    }
    auto available_command =
            std::find_if(available_commands_.begin(),
                         available_commands_.end(),
                         [command](const Command& command_){return command_.command == command;});
    if(available_command != available_commands_.end()){
        int max_arg_count = available_command->max_argument_count;
        int current_arg_count = static_cast<int>(args.size());
        if(max_arg_count != current_arg_count){
            if(!(args.empty() && available_command->max_argument_count > 0)){
                throw std::logic_error("Invalid number of arguments");
            }
        }
        if(has_correct_types(*available_command, args)){
            Command ret_command(*available_command);
            ret_command.args = args;
            return ret_command;
        }
        else{
            throw std::logic_error("Incompatible type provided");
        }
    }
    throw std::logic_error("This is not a valid command");
}

bool CommandParser::has_correct_types(const Command& command, std::vector<Arg>& args)
{
    for(size_t i = 0; i < args.size(); ++i){
        switch (command.valid_types[i]) {
        case INT:{
            for(char c : args[i].string_arg){
                if(!isdigit(c)){
                    return false;
                }
            }
            args[i].int_arg = std::stoi(args[i].string_arg);
            break;
        }
        case FLOAT:{
            for(char c : args[i].string_arg){
                if(!isdigit(c) && c != '.'){
                    return false;
                }
            }
            args[i].float_arg = std::stod(args[i].string_arg);
            break;
        }
        case STRING:
            break;
        }
    }
    return true;
}
