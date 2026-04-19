#ifndef CommandParser_h
#define CommandParser_h

#include <string>
#include <vector>

class CommandParser {
public:
    // Constructor
    CommandParser();
    
    // Method to parse a command
    void parseCommand(const std::string& command);
    
    // Method to get the list of commands
    std::vector<std::string> getCommands() const;

private:
    std::vector<std::string> commands;
};

#endif // CommandParser_h
