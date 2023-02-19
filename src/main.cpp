#include "ncurses_display.h"
#include "system.h"
#include "linux_parser.h"
#include <iostream>

using namespace std;

int main() {
  // cout << LinuxParser::UpTime(1330) << "\n";
  // vector<string> cpuUU=LinuxParser::CpuUtilization();
  // for (const auto& s : cpuUU) {
  //   cout << s << " ";
  // }
  // cout << endl;
  System system;
  NCursesDisplay::Display(system);
}