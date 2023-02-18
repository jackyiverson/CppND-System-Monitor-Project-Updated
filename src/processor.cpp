#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "processor.h"
#include "linux_parser.h"

using namespace LinuxParser;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    std::string line, key;
    std::ifstream stream(kProcDirectory + kStatFilename);
    std::getline(stream, line);
    
    std::istringstream iss(line);
    std::string cpu_name;
    int user, nice, sys, idle, iowait, irq, softirq, steal, guest, guest_nice;
    iss >> cpu_name >> user >> nice >> sys >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
    int idle_time = idle + iowait;
    int total_time = user + nice + sys + idle_time + irq + softirq + steal;
    float utilization = static_cast<float> (total_time - idle_time) / total_time;
  
    return utilization;
}