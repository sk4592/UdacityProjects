#include "ncurses_display.h"
#include "system.h"
#include "linux_parser.h"
#include <iostream>
#include <vector>
using std::vector;

int main() {
  System system;
  NCursesDisplay::Display(system);
  
  /*
  vector<int> data{1692, 1306};
  for (auto i : data) {
    std::cout << LinuxParser::User(i) << "\n";
  }
  */
}
