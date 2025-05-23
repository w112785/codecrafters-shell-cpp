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
    if (input == "exit")
    {
      break;
    }
    else{
    std::cout << input << ": command not found" << std::endl;
    }
  }
}
