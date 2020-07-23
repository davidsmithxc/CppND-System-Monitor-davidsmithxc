#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string>
#include <map>
#include <iostream>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {

    std::vector<std::string> proc_stat_info;
    std::vector<std::string> proc_cpu_info_keys{"user",
                                                "nice",
                                                "system",
                                                "idle",
                                                "iowait",
                                                "irq",
                                                "softirq",
                                                "guest",
                                                "guest_nice"
                                                };
    // std::unordered_map<std::string, float> cpu_info_map;
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
    float totaltime = usertime + nicetime + systemalltime + idlealltime + virtalltime;
    float non_idletime = usertime + nicetime + irqtime + softirqtime;

    return non_idletime / totaltime;
}