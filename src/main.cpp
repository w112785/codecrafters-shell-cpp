#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <filesystem>

std::vector<std::string> SplitString(const std::string& str, char delimiter = ' ') {
  std::istringstream iss(str);
  std::vector<std::string> tokens;
  std::string token;
  while (std::getline(iss, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

bool SearchDirectorys(const std::vector<std::string> &paths, std::string& file){
  std::string checkfile;
  for (std::string path: paths){
    checkfile = path + file;
    if (std::filesystem::exists(checkfile)){
      std::cout << file << " is " << path << "\n";
      return true;
    }
  }
  return false;
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::string input;
  std::vector<std::string> command;

  while (true)
  {
    // Uncomment this block to pass the first stage
    std::cout << "$ ";
    std::getline(std::cin, input);

    command = SplitString(input);
    //if (input == "exit")
    if (command[0] == "exit") { return 0; }
    else if (command[0] == "echo"){
      std::cout << input.substr(5) << std::endl;
    }
    else if (command[0] == "type") {
      if (command.size() > 1) {
        if (command[1] == "echo" || command[1] == "exit" || command[1] == "type") {
          std::cout << command[1] << " is a shell builtin" << std::endl;
        }
        else {
          std::vector<std::string> path;
          // becuase I run windows and Windows and linux run different path seperators
#ifdef _WIN32
          path = SplitString(std::getenv("PATH"), ';');
#elif defined(_POSIX_VERSION)
          path = SplitString(std::getenv("PATH"), ':');
#endif
          if (!SearchDirectorys(path, command[1])) {
            std::cout << command[1] << ": not found" << std::endl;
          }
        }
      }
      else{
          std::cout << "Usage: type [name]" << std::endl;
      }
    }
    else{
      std::cout << input << ": command not found" << std::endl;
    }
  }
}
