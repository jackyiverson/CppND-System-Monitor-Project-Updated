#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


#include "processor.h"
#include "linux_parser.h"

using namespace LinuxParser;
using namespace std;

// TODO: Return the aggregate CPU utilization
// float Processor::Utilization() {
//     std::string line;
//     std::ifstream stream(kProcDirectory + kStatFilename);
//     std::getline(stream, line);
    
//     std::istringstream iss(line);
//     std::string cpu_name;
//     int user, nice, sys, idle, iowait, irq, softirq, steal, guest, guest_nice;
//     iss >> cpu_name >> user >> nice >> sys >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
//     long idle_time = idle + iowait;
//     long total_time = user + nice + sys + idle_time + irq + softirq + steal;
//     float utilization = 1.0 * (total_time - idle_time) / total_time;

//     prevTotal_ = total_time;
//     prevIdle_ = idle_time;
  
//     return utilization;
// }
float Processor::Utilization() {
  std::vector<std::string> cpu_stats = LinuxParser::CpuUtilization();
  if (cpu_stats.size() < 8) {
    return 0.0;
  }

  long user = std::stol(cpu_stats[0]);
  long nice = std::stol(cpu_stats[1]);
  long system = std::stol(cpu_stats[2]);
  long idle = std::stol(cpu_stats[3]);
  long iowait = std::stol(cpu_stats[4]);
  long irq = std::stol(cpu_stats[5]);
  long softirq = std::stol(cpu_stats[6]);
  long steal = std::stol(cpu_stats[7]);

  float total = user + nice + system + idle + iowait + irq + softirq + steal;
  float idle_time = idle + iowait;
  float usage = (total - idle_time) / total;

  return usage;
}