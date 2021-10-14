#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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
  string os, kernel, temp;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> temp >> kernel;
  }
  return temp + " "+ kernel;
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
  string key;
  long value[2]{0, 0};
  std::string line;
  std::ifstream memfile(kProcDirectory + kMeminfoFilename);
  if (memfile.is_open()) {
    for (auto i=0; i<2; i++) {
      getline(memfile, line);
      std::istringstream LineParse(line);
      LineParse >> key >> value[i];
    }
  }
  float used_mem = abs(value[0] - value[1]);
  float res = float (used_mem / value[0]);
  return res;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  long uptime, idletime;
  std::string line;
  std::ifstream memfile(kProcDirectory + kUptimeFilename);
  if (memfile.is_open()) {
    getline(memfile, line);
    std::istringstream LineParse(line);
    LineParse >> uptime >> idletime;
  }
  return uptime;
 }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return LinuxParser::IdleJiffies() + LinuxParser::ActiveJiffies();
   
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  std::string line, temp, res;
  long utime, stime{0}, cutime{0}, cstime{0};
  std::ifstream mystream(kProcDirectory + std::to_string(pid) + kStatFilename);
  getline(mystream, line);
  std::istringstream lineparse(line);
  int n = 0;
  while (lineparse >> temp) {
    if (n == 13) {  utime = std::stol(temp);}
    if (n == 14) {  stime = std::stol(temp);}
    if (n == 15) {  cutime = std::stol(temp);}
    if (n == 16) {  cstime = std::stol(temp);}
    n += 1;
  }
  return (utime + stime + cutime + cstime); 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  vector<string> data = LinuxParser::CpuUtilization();
  return stol(data[0]) + stol(data[1]) + stol(data[2]) + stol(data[5]) + stol(data[6]) + stol(data[7]);
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> data = LinuxParser::CpuUtilization();
  return std::stol(data[3]) + std::stol(data[4]);
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> data{};
  std::string line;
  std::ifstream memfile(kProcDirectory + kStatFilename);
  if (memfile) {
    getline(memfile, line); //cpu
    //getline(memfile, line); //cpu 0
    //getline(memfile, line); //cpu 1
    std::istringstream LineParser(line);
    LineParser >> line;
    if (line == "cpu"){
      while (LineParser >> line) 
      {
        if (line != " ") 
          data.push_back(line);
      }
    }
  }
  return data;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  std::string line, str;
  std::ifstream stream(kProcDirectory + kStatFilename);
  while (getline(stream, line)) {
    std::istringstream LineParser(line);
    LineParser >> str;
    if (str == "processes") {
      LineParser >> str;
      return stoi(str);
    }
  } 
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
    std::string line, str;
  std::ifstream stream(kProcDirectory + kStatFilename);
  while (getline(stream, line)) {
    std::istringstream LineParser(line);
    LineParser >> str;
    if (str == "procs_running") {
      LineParser >> str;
      return stoi(str);
    }
  }
  return 0; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  std::string line;
  std::fstream LineParse (kProcDirectory + std::to_string(pid) +kCmdlineFilename); 
  getline(LineParse, line);
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  std::string line, temp, test;
  std::stringstream ss;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  while(getline(stream, line)) {
    std::istringstream lineparse(line);
    while (lineparse >> temp) {
      if (temp == "VmSize:"){
        lineparse >> test;
        long tmp = std::stol(test);
        float res = (tmp * 0.001);
        ss << std::fixed << std::setprecision(1) << std::round(res);
        return ss.str(); //std::to_string(res);
      }
    }
  }
  return " ";
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  std::string line{}, temp{}, res{};
  std::ifstream mystream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  while(getline(mystream, line)) {
    std::istringstream lineparse(line);
    while(lineparse >> temp) {
      if (temp == "Uid:") {
        lineparse >> temp;
        return temp;
      }
    }
  }
  return " ";
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  std::string line{}, temp[2]{}, fn{}, res{};
  std::ifstream mystream(kPasswordPath);
  while(getline(mystream, line)) {
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream lineparse(line);
    while(lineparse >> fn >> temp[0] >> temp[1]) {
      if (temp[1] == LinuxParser::Uid(pid)) {
        res = fn;
        return res;
      }
    }
  }
  return " ";
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  std::string line, temp;
  long starttime{0};
  std::ifstream LineParse (kProcDirectory + to_string(pid) + kStatFilename);
  getline(LineParse, line);
  std::istringstream stream(line);
  int n = 0;
  while (stream >> temp) {
    if (n == 21) { 
      starttime = std::stol(temp); 
    }
    n += 1;
  }
  long uptime = (LinuxParser::UpTime());
  return uptime - (starttime / sysconf(_SC_CLK_TCK));
  
}