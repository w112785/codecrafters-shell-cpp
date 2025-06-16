#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <filesystem>

#include "Modes.hpp"

std::vector<std::string> SplitString(const std::string& str, char delimiter, bool searchForQuotes = false) {
  std::istringstream iss(str);
  std::vector<std::string> tokens;
  std::string token;
  if (searchForQuotes){
    bool inSingleQuotes = false;

    for (size_t i = 0; i < str.length(); i++) {
      char c = str[i];

      if (c == '\'') {
        if (!inSingleQuotes) {
          inSingleQuotes = true;
        } else {
          inSingleQuotes = false;
        }
      } else if (c == delimiter && !inSingleQuotes) {
        if (!token.empty()) {
          tokens.push_back(token);
          token.clear();
        }
      } else {
        token += c;
      }
    }
    if (!token.empty()) {
      tokens.push_back(token);
      token.clear();
    }
  } else {
    while (std::getline(iss, token, delimiter)) {
      tokens.push_back(token);
      token.clear();
    }
  }
  return tokens;
}

bool SearchDirectorys(const std::vector<std::string>& paths, const std::string& file, std::string& outstr) {
  std::string checkfile;
  for (std::string path : paths) {
    // On windows there is a trailing dir seperator, On linux there is not so it was looking for cat with /bincat and not /bin/cat.
    // this seems to work both on windows and linux. So leaving it in.
    checkfile = path + "/" + file;
    if (std::filesystem::exists(checkfile)) {
      outstr = checkfile;
      return true;
    }
  }
  return false;
}

// This was added to keep things better organized. 
// As I continue this challenge I am finding I need to keep this code scalable.
struct BuiltinCommand { 
  std::string name;
  std::string usage;
  bool (*exeCommand)(std::vector<std::string> cmd);
};

std::vector<BuiltinCommand> builtins;
std::vector<std::string> path;
std::string input;

bool pwd(std::vector<std::string> cmd){
  std::cout << std::filesystem::current_path().string() << "\n";
  return true;
}

bool CmdType(std::vector<std::string> cmd){
  if (cmd.size() > 1) {
    for (BuiltinCommand builtin: builtins){
      if (cmd[1] == builtin.name){
        std::cout << cmd[1] << " is a shell builtin" << std::endl;
        return true;
      }
    }
    std::string typeresult;
    if (!SearchDirectorys(path, cmd[1], typeresult)) {
      std::cout << cmd[1] << ": not found" << std::endl;
    }
    else {
      std::cout << cmd[1] << " is " << typeresult << std::endl;
    }
    return true;
  }
  else {
    return false;
  }
}

bool CmdEcho(std::vector<std::string> cmd){
  if (cmd.size() <= 1){
    return false;
  }
  for (int x = 1; x <= (cmd.size() - 1); x++){
    std::cout << cmd[x] << " ";
  }
  std::cout << std::endl;
  return true;
}

bool CmdCd(std::vector<std::string> cmd){
  if (std::filesystem::exists(cmd[1])){
    std::filesystem::current_path(cmd[1]);
  }
  else if (cmd[1] == "~"){
    std::filesystem::current_path(std::getenv("HOME"));
  }
  else{
    std::cout << "cd: " << cmd[1] << ": No such file or directory" << std::endl;
  }
  return true;
}

bool CmdExit(std::vector<std::string> cmd){return false;} //was created but after rethough shouild never be called.
                                                          // keeping to create a pointer for the exit call

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::vector<std::string> command;
  bool command_found;
  // becuase I run windows and Windows and linux run different path seperators
#ifdef _WIN32
  path = SplitString(std::getenv("PATH"), ';');
#else
  path = SplitString(std::getenv("PATH"), ':');
#endif

  builtins.push_back({"exit", " ", *CmdExit});
  builtins.push_back({"echo", "Usage: echo [text or numbers to echo to the screen] \nFuture will implement a way to get Enviromnet Variables from this command \nEG. $PATH", *CmdEcho});
  builtins.push_back({"pwd", " ", *pwd});
  builtins.push_back({"type", "Usage: type [name]", *CmdType });
  builtins.push_back({"cd", " ", *CmdCd});

  while (true) {
    std::cout << "$ ";
    std::getline(std::cin, input);

    command_found = false;
    command = SplitString(input, ' ', true);

    for (BuiltinCommand builtincmd: builtins) {
      if (command[0] == builtincmd.name) {
        if (builtincmd.name == "exit"){
          return 0;
        }
        if (!builtincmd.exeCommand(command)) {
          std::cout << builtincmd.usage << std::endl;
        }
        command_found = true;
      }
    }
    if (!command_found){
      std::string exeCommand;
      if (SearchDirectorys(path, command[0], exeCommand)) {
        system(input.c_str());
        continue;
      }
      std::cout << input << ": command not found" << std::endl;
    }
    /*
    //if (input == "exit")
    if (command[0] == "exit") { return 0; }
    else if (command[0] == "echo"){
      std::cout << input.substr(5) << std::endl;
    }
    else if (command[0] == "pwd"){
      std::cout << std::filesystem::current_path().string() << "\n";
    }
    else if (command[0] == "type") {
      if (command.size() > 1) {
        if (command[1] == "echo" || command[1] == "exit" || command[1] == "type" || command[1] == "pwd") {
          std::cout << command[1] << " is a shell builtin" << std::endl;
        }
        else {
          std::string typeresult;
          if (!SearchDirectorys(path, command[1], typeresult)) {
            std::cout << command[1] << ": not found" << std::endl;
          }
          else {
            std::cout << command[1] << " is " << typeresult << "\n";
          }
        }
      }
      else{
          std::cout << "Usage: type [name]" << std::endl;
      }
    }
    else{
      std::string exeCommand;
      if (SearchDirectorys(path, command[0], exeCommand)){
        system(input.c_str());
        continue;
      }

      std::cout << input << ": command not found" << std::endl;
    }
    */
  }
}
