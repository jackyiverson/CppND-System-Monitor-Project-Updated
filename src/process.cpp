#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid): pid_(pid){
    cpu_utilization_ = CpuUtilization();
}

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    std::ifstream stream(LinuxParser::kProcDirectory + to_string(Pid()) + LinuxParser::kStatFilename);
    string line;
    string value;
    float uptime;
    float starttime;
    float total_time;
    
    if (stream.is_open()) {
     getline(stream, line);
     stream.close();
     vector<string> values;
     std::istringstream linestream(line);
        while (linestream >> value) {
        values.push_back(value);
        }
        // Extract relevant data from /proc/[pid]/stat file
        uptime = LinuxParser::UpTime(Pid());
        total_time = stof(values[13]) + stof(values[14]) + stof(values[15]) + stof(values[16]);
        starttime = stof(values[21]);
    }

    // Calculate process CPU utilization
    float seconds = uptime - starttime / sysconf(_SC_CLK_TCK);
    float cpu_usage = (total_time / sysconf(_SC_CLK_TCK)) / seconds;

    return cpu_usage;
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Pid()); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(Pid()); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Pid()); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const {
    return a.cpu_utilization_ < cpu_utilization_;
}