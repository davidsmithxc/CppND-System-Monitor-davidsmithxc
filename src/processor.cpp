#include "processor.h"

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "linux_parser.h"

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  std::vector<std::string> proc_stat_info;
  std::vector<float> cpu_info;

  // get cpu aggregate info
  proc_stat_info = LinuxParser::CpuUtilization();
  // convert to mapped float information
  for (unsigned int i = 0; i < proc_stat_info.size(); i++) {
    float value = std::stof(proc_stat_info[i]);
    cpu_info.push_back(value);
  }

  float usertime = cpu_info[LinuxParser::CPUStates::kUser_];
  float nicetime = cpu_info[LinuxParser::CPUStates::kNice_];
  float systemtime = cpu_info[LinuxParser::CPUStates::kSystem_];
  float idletime = cpu_info[LinuxParser::CPUStates::kIdle_];
  float iowaittime = cpu_info[LinuxParser::CPUStates::kIOwait_];
  float irqtime = cpu_info[LinuxParser::CPUStates::kIRQ_];
  float softirqtime = cpu_info[LinuxParser::CPUStates::kSoftIRQ_];
  float guesttime = cpu_info[LinuxParser::CPUStates::kGuest_];
  float guest_nicetime = cpu_info[LinuxParser::CPUStates::kGuestNice_];

  // implement calculation
  usertime -= guesttime;
  nicetime -= guest_nicetime;
  float idlealltime = idletime + iowaittime;
  float systemalltime = systemtime + irqtime + softirqtime;
  float virtalltime = guesttime + guest_nicetime;
  float totaltime = usertime + nicetime + systemalltime + idlealltime + virtalltime;
  float non_idletime = usertime + nicetime + irqtime + softirqtime;

  float delta_alltime = totaltime - m_prev_alltime;
  float delta_nonidletime = non_idletime - m_prev_nonidletime;

  m_prev_alltime = totaltime;
  m_prev_nonidletime = non_idletime;

  return delta_nonidletime / delta_alltime;
}