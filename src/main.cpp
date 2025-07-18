#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <filesystem>
#include <format>

#include "Modes.hpp"
#include "tools.hpp"


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

bool pwd(std::vector<std::string> cmd) {
  std::cout << std::filesystem::current_path().string() << "\n";
  return true;
}

bool CmdType(std::vector<std::string> cmd) {
  if (cmd.size() > 1) {
    for (BuiltinCommand builtin : builtins) {
      if (cmd[1] == builtin.name) {
        std::cout << cmd[1] << " is a shell builtin" << std::endl;
        return true;
      }
    }
    std::string typeresult;
    if (!SearchDirectorys(path, cmd[1], typeresult)) {
      std::cout << cmd[1] << ": not found" << std::endl;
    } else {
      std::cout << cmd[1] << " is " << typeresult << std::endl;
    }
    return true;
  } else {
    return false;
  }
}

bool CmdEcho(std::vector<std::string> cmd) {
  if (cmd.size() <= 1) {
    return false;
  }
  for (int x = 1; x <= (cmd.size() - 1); x++) {
    std::cout << cmd[x] << " ";
  }
  std::cout << std::endl;
  return true;
}

bool CmdCd(std::vector<std::string> cmd) {
  if (std::filesystem::exists(cmd[1])) {
    std::filesystem::current_path(cmd[1]);
  } else if (cmd[1] == "~") {
    std::filesystem::current_path(std::getenv("HOME"));
  } else {
    std::cout << "cd: " << cmd[1] << ": No such file or directory" << std::endl;
  }
  return true;
}

bool CmdExit(std::vector<std::string> cmd) { return false; } //was created but after rethought shouild never be called.
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

  builtins.push_back({ "exit", " ", *CmdExit });
  builtins.push_back({ "echo", "Usage: echo [text or numbers to echo to the screen] \nFuture will implement a way to get Enviromnet Variables from this command \nEG. $PATH", *CmdEcho });
  builtins.push_back({ "pwd", " ", *pwd });
  builtins.push_back({ "type", "Usage: type [name]", *CmdType });
  builtins.push_back({ "cd", " ", *CmdCd });

  while (true) {
    std::cout << "$ ";
    std::getline(std::cin, input);

    command_found = false;
    command = SplitString(input, ' ', true);
    if (command.empty()) {
      continue;
    }

    for (BuiltinCommand builtincmd : builtins) {
      if (command[0] == builtincmd.name) {
        if (builtincmd.name == "exit") {
          if (command.size() > 1) { // edited so I can actually pass the exit challenge. even though it only does 0 for the test.
            try {
              int exitCode = std::stoi(command[1]);
              if (exitCode >= 0 && exitCode <= 255) {
                return exitCode;
              }
            } catch (std::invalid_argument) {
              return 0;
            } catch (std::out_of_range) {
              return 1;
            }
          } else { return 0; }
        }
        if (!builtincmd.exeCommand(command)) {
          std::cout << builtincmd.usage << std::endl;
        }
        command_found = true;
      }
    }
    if (!command_found) {
      std::string exeCommand;
      if (SearchDirectorys(path, command[0], exeCommand)) {
        system(input.c_str());
        continue;
      }
      std::cout << command[0] << ": command not found" << std::endl;
    }
  }
}
