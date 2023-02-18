#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "processor.h"
#include "linux_parser.h"

using namespace LinuxParser;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    std::string line;
    std::ifstream stream(kProcDirectory + kStatFilename);
    std::getline(stream, line);
    
    std::istringstream iss(line);
    std::string cpu_name;
    int user, nice, sys, idle, iowait, irq, softirq, steal, guest, guest_nice;
    iss >> cpu_name >> user >> nice >> sys >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
    long idle_time = idle + iowait;
    long total_time = user + nice + sys + idle_time + irq + softirq + steal;
    float utilization = 1.0 * (total_time - idle_time) / total_time;

    IdleP = idle_time;
    TotalP = total_time;
  
    return utilization;
}