#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string>
#include <numeric>
#include <iostream>
using std::string;
using std::stof;

// TODO: Return the aggregate CPU utilization

float Processor::Utilization() { 
    float res = 0;
    float idle = LinuxParser::IdleJiffies();
    float nonidle = LinuxParser::ActiveJiffies();
    float totalsum = idle + nonidle;

    float totald = totalsum - prevsum;
    float idled = idle - previdle;
    res = (totald - idled) / totald;
    
    previdle = idle;
    prevnonidle = nonidle;
    prevsum = totalsum;
    return res;
}