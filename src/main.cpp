#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <filesystem>

// This was added to keep things better organized. 
// As I continue this challenge I am finding I need to keep this code scalable.
struct command { // TODO: Implement in the code and reference off this not hardcoded strings
  std::string name;
  int params;
  std::string usage;
};

std::vector<std::string> SplitString(const std::string& str, char delimiter = ' ') {
  std::istringstream iss(str);
  std::vector<std::string> tokens;
  std::string token;
  while (std::getline(iss, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

bool SearchDirectorys(const std::vector<std::string> &paths, const std::string& file, std::string &outstr){
  std::string checkfile;
  for (std::string path: paths){
    // On windows there is a trailing dir seperator, On linux there is not so it was looking for cat with /bincat and not /bin/cat.
    // this seems to work both on windows and linux. So leaving it in.
    checkfile = path + "/" + file;
    if (std::filesystem::exists(checkfile)){
      outstr = checkfile;
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
  std::vector<std::string> path;
  // becuase I run windows and Windows and linux run different path seperators
#ifdef _WIN32
  path = SplitString(std::getenv("PATH"), ';');
#else
  path = SplitString(std::getenv("PATH"), ':');
#endif

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
    else if (command[0] == "pwd"){
      std:: cout << std::filesystem::current_path() << "\n";
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
  }
}
