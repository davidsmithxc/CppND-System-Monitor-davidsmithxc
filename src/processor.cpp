#include "processor.h"

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "linux_parser.h"

// DONE: Return the aggregate CPU utilization
// TODO: Clean up CPU utilization code -> Use Linux Parser Enum CpuUtilization
float Processor::Utilization() {
  std::vector<std::string> proc_stat_info;
  /* The Utilization Data is reported in the following order
  "user",
  "nice",
  "system",
  "idle",
  "iowait",
  "irq",
  "softirq",
  "guest",
  "guest_nice"
  */

  std::vector<float> cpu_info;

  // get cpu aggregate info
  proc_stat_info = LinuxParser::CpuUtilization();
  // convert to mapped float information
  for (unsigned int i = 0; i < proc_stat_info.size(); i++) {
    float value = std::stof(proc_stat_info[i]);
    cpu_info.push_back(value);
  }

  float usertime = cpu_info[0];
  float nicetime = cpu_info[1];
  float systemtime = cpu_info[2];
  float idletime = cpu_info[3];
  float iowaittime = cpu_info[4];
  float irqtime = cpu_info[5];
  float softirqtime = cpu_info[6];
  float guesttime = cpu_info[7];
  float guest_nicetime = cpu_info[8];

  // implement calculation
  usertime -= guesttime;
  nicetime -= guest_nicetime;
  float idlealltime = idletime + iowaittime;
  float systemalltime = systemtime + irqtime + softirqtime;
  float virtalltime = guesttime + guest_nicetime;
  float totaltime =
      usertime + nicetime + systemalltime + idlealltime + virtalltime;
  float non_idletime = usertime + nicetime + irqtime + softirqtime;

  return non_idletime / totaltime;
}