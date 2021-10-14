#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <vector>
#include <string>
using std::string;

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
 std::vector<string> prevdata{};
 float prevnonidle{0};
 float previdle{0};
 float prevsum{0};
};

#endif