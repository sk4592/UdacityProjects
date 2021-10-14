#include <iostream>
#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"


using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid)
{
    cpu_usage = CpuUtilization();
}

// TODO: Return this process's ID
int Process::Pid() { 
    return pid_;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    std::string line, temp;
    long uptime, starttime;
    std::ifstream LineParse(LinuxParser::kProcDirectory + std::to_string(Pid()) + LinuxParser::kStatFilename); 
    getline(LineParse, line);
    std::istringstream mystream(line);
    int  i = 0;
    uptime = LinuxParser::UpTime();
    while (mystream >> temp) {
       if (i == 21) {  starttime = std::stol(temp);}
       i += 1;
    }
    float hertz = sysconf(_SC_CLK_TCK);
    float total_time = LinuxParser::ActiveJiffies(Pid());
    float seconds = uptime - (starttime / hertz);
    float tmp = (total_time / hertz);
    cpu_usage = ( tmp / seconds);
    return cpu_usage; 
}

// TODO: Return the command that generated this process
string Process::Command() { 
  std::string line, temp;
  std::ifstream LineParse (LinuxParser::kProcDirectory + std::to_string(Pid()) + LinuxParser::kCmdlineFilename); 
  getline(LineParse, line);
  return line;
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
    return LinuxParser::Ram(Pid()); 
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
    return LinuxParser::User(Pid()); 
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
   return  LinuxParser::UpTime(Pid());   
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    return cpu_usage < a.cpu_usage;
}