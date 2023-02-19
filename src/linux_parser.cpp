#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <regex>
#include <iomanip>
#include <fstream>
#include <map>
#include "linux_parser.h"


using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::ifstream;
using std::istringstream;
using std::getline;
using std::cout;
using std::sort;
using std::reverse;
using std::setw;
using std::left;
using std::right;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line, key;
  float value, MemTotal, MemFree;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "MemTotal:"){
        MemTotal = value;

      }
      else if (key == "MemFree:"){
        MemFree = value;
      }    
    }
  }
  return (MemTotal - MemFree) / MemTotal;      
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  long uptime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()){
    getline(stream, line);
    istringstream linestream(line);
    linestream >> uptime;
  }
  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::string line;
    std::getline(filestream, line);
    std::istringstream linestream(line);
    std::string cpu_label;
    long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    linestream >> cpu_label >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
    return user + nice + system + idle + iowait + irq + softirq + steal + guest + guest_nice;
  }
  return 0;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  std::string line, value;
  long utime, stime, cutime, cstime;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 13; ++i) {
      linestream >> value;
    }
    linestream >> utime >> stime >> cutime >> cstime;
  }
  return utime + stime + cutime + cstime;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::string line;
    std::getline(filestream, line);
    std::istringstream linestream(line);
    std::string cpu_label;
    long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    linestream >> cpu_label >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
    long total = user + nice + system + idle + iowait + irq + softirq + steal + guest + guest_nice;
    long idle_time = idle + iowait;
    return total - idle_time;
  }
  return 0;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::string line;
    std::getline(filestream, line);
    std::istringstream linestream(line);
    std::string cpu_label;
    long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    linestream >> cpu_label >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
    return idle + iowait;
  }
  return 0;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpu_utilization;
  string line, cpu;
  ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    getline(filestream, line);
    istringstream linestream(line);
    linestream >> cpu;
    for (string utilization; linestream >> utilization;)
      cpu_utilization.emplace_back(utilization);
  }
  return cpu_utilization;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, key;
  int value{0};
  std::ifstream stream(kProcDirectory + kStatFilename);
  while (std::getline(stream, line)){
    istringstream linestream(line);
    linestream >> key >> value;
    if (key == "processes"){
      return value;
    }
  }
  return value;
}
 


// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, key;
  int value{0};
  std::ifstream stream(kProcDirectory + kStatFilename);
  while (std::getline(stream, line)){
    istringstream linestream(line);
    linestream >> key >> value;
    if (key == "procs_running"){
      return value;
    }
  }
  return value;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::string line;
    std::getline(filestream, line);
    return line;
  }
  return "string()";
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  std::string ram;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    std::string line;
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      std::string key;
      linestream >> key;
      if (key == "VmSize:") {
        long value_kb;
        linestream >> value_kb;
        long value_mb = value_kb / 1024;
        ram = std::to_string(value_mb);
        break;
      }
    }
  }
  return ram;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    std::string line;
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      std::string key;
      std::string uid;
      linestream >> key >> uid;
      if (key == "Uid:") {
        return uid;
      }
    }
  }
  return string();
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  std::string uid = Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    std::string line;
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      std::string username, x, user_id;
      linestream >> username >> x >> user_id;
      if (user_id == uid) {
        return username;
      }
    }
  }
  return string();
}


// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  long uptime = LinuxParser::UpTime(); // Get system uptime

  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::string line;
    std::getline(stream, line);
    std::istringstream linestream(line);
    std::istream_iterator<std::string> beg(linestream), end;
    std::vector<std::string> stat_values(beg, end);

    if (stat_values.size() > 21) {
      long starttime = std::stol(stat_values[21]);
      long process_uptime = uptime - (starttime / sysconf(_SC_CLK_TCK));
      return process_uptime;
    }
  }

  return 0; // If something goes wrong, return 0
}
