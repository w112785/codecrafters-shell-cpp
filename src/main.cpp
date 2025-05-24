#include <iostream>
#include <string>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::string input;

  while (true)
  {
    // Uncomment this block to pass the first stage
    std::cout << "$ ";
    std::getline(std::cin, input);
    //if (input == "exit")
    if (input.starts_with("exit"))    {      return 0;    }
    else if (input.starts_with("echo")){
      std::cout << input.substr(5) << std::endl;
    }
    else if (input.starts_with("type"))
      if (input.substr(0, 9) == "type echo" || input.substr(0, 9) == "type exit" || input.substr(0, 9) == "type type") {
        std::cout << input.substr(5) + " is a shell builtin" << std::endl;
      }
      else {
        std::cout << input.substr(5) + ": not found" << std::endl;
      }
    else{
    std::cout << input << ": command not found" << std::endl;
    }
  }
}
