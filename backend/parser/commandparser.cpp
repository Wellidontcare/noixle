#include "commandparser.h"

CommandParser::CommandParser(std::vector<Command> available_commands)
        : available_commands_(std::move(available_commands)) {}

Command CommandParser::parse(const char *input) {

    std::stringstream input_stream(input);
    std::string command;
    std::vector<Arg> args;
    input_stream >> command;
    std::string arg;
    if (command == "echo") {
        QString echo_str = input_stream.str().c_str();
        echo_str = echo_str.remove(0, 4);
        args.emplace_back(Arg{0, 0, echo_str.toStdString()});
    } else {

        while (input_stream >> arg) {
            args.emplace_back(Arg{0, 0, arg});
        }
    }
    auto available_command =
            std::find_if(available_commands_.begin(),
                         available_commands_.end(),
                         [command](const Command &command_) { return command_.command == command; });
    if (available_command != available_commands_.end()) {
        int max_arg_count = available_command->max_argument_count;
        int current_arg_count = static_cast<int>(args.size());
        if (max_arg_count < current_arg_count &&
            !(current_arg_count == 0 && available_command->null_argument_callable)) {
            throw std::logic_error("Error in " + std::string(__func__) + "\nInvalid number of arguments");
        }
        if (current_arg_count < max_arg_count && !available_command->null_argument_callable) {
            throw std::logic_error("Error in " + std::string(__func__) + "\nThis command requires more arguments");
        }
        if (has_correct_types(*available_command, args) || command == "echo") {
            Command ret_command(*available_command);
            ret_command.args = args;
            return ret_command;
        } else {
            throw std::logic_error("Error in " + std::string(__func__) + "\nIncompatible type provided");
        }
    }
    throw std::logic_error("Error in " + std::string(__func__) + "\nThis is not a valid command");
}

bool CommandParser::has_correct_types(const Command &command, std::vector<Arg> &args) {
    for (size_t i = 0; i < args.size(); ++i) {
        switch (command.valid_types[i]) {
            case INT: {
                for (char c : args[i].string_arg) {
                    if (!isdigit(c) && (c != '-') && (c != '+')) {
                        return false;
                    }
                }
                args[i].int_arg = std::stoi(args[i].string_arg);
                break;
            }
            case FLOAT: {
                for (char c : args[i].string_arg) {
                    if (!isdigit(c) && (c != '.') && (c != '-') && (c != '+')) {
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
