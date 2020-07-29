#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <cmath>
#include <string>
#include <vector>

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
  string os, version, kernel;
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
        // not checking empty string give if clause check for isdigit
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  string value;
  std::unordered_map<string, float> meminfo_map;

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (value != "") {
        meminfo_map[key] = std::stof(value);
      } else {
        meminfo_map[key] = 0.0f;
      }
    };
  }

  return (1.0 - meminfo_map["MemFree"] / meminfo_map["MemTotal"]);
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  string uptime, line;

  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }

  return std::lround(std::stof(uptime));
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// Done: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  std::string line, value;
  long int utime = 0, stime = 0, cutime = 0, cstime = 0;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);

  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream stringstream(line);

    for (int i = 1; i <= 17; i++) {
      stringstream >> value;

      if (i == 14 && value != "") {
        utime = (long)std::stoi(value);
      }
      if (i == 15 && value != "") {
        stime = (long)std::stoi(value);
      }
      if (i == 16 && value != "") {
        cutime = (long)std::stoi(value);
      }
      if (i == 17 && value != "") {
        cstime = (long)std::stoi(value);
      }
    }
  }

  return utime + stime + cutime + cstime;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  std::string line, key, value;
  std::vector<std::string> cpu_util_info;

  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream stringstream(line);
    stringstream >> key;
    if (key == "cpu") {
      while (stringstream >> value) {
        cpu_util_info.push_back(value);
      }
    }
  }

  return cpu_util_info;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::string line, key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream stringstream(line);
      stringstream >> key >> value;
      if (key == "processes" && value != "") {
        return std::stoi(value);
      }
    }
  }
  return 0;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::string line, key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream stringstream(line);
      stringstream >> key >> value;
      if (key == "procs_running" && value != "") {
        return std::stoi(value);
      }
    }
  }
  return 0;
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  std::string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kCmdlineFilename);

  if (filestream.is_open()) {
    std::getline(filestream, line);
    if (!line.empty()) return line;
  }
  return std::string("N/A");
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  std::string line, key, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream stringstream(line);

      stringstream >> key >> value;

      if (key == "VmSize") {
        int ram_mb = 0;
        if(value != "") { ram_mb = std::stof(value) / 1000.0f; }
        return (std::to_string(ram_mb));
      }
    }
  }

  // Intentionally return 0.0 instead of N/A to simplify comparisons
  return std::string("0.0");
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  std::string line, key, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream stringstream(line);
      stringstream >> key >> value;
      if (key == "Uid") {
        return value;
      }
    }
  }
  return string();
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  std::string uid_string = Uid(pid);
  int uid_int = 0;
  if (uid_string != "") {
    uid_int = std::stoi(uid_string); // not checking for empty string as string is guaranteed from above
  } else {
    return "ERROR";
  }

  // init map on first call
  if (LinuxParser::uid_map.empty()) {
    std::string line, usr_name, usr_x, usr_num;
    std::ifstream filestream(kPasswordPath);

    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream stringstream(line);
        stringstream >> usr_name >> usr_x >> usr_num;
        if (usr_num != "") {
          LinuxParser::uid_map[std::stoi(usr_num)] = std::string(usr_name);
        }
      }
    }
  }

  // Subsequently return uname if in map, else add to map
  if (LinuxParser::uid_map.find(pid) == LinuxParser::uid_map.end()) {
    std::string line, usr_name, usr_x, usr_num;
    std::ifstream filestream(kPasswordPath);
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream stringstream(line);
        stringstream >> usr_name >> usr_x >> usr_num;
        if (usr_num == uid_string) {
          LinuxParser::uid_map[uid_int] = std::string(usr_name);
          break;
        }
      }
    }
  }

  return LinuxParser::uid_map[uid_int];
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  std::string line, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);

  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream stringstream(line);

    for (int i = 0; i <= 21; i++) {
      stringstream >> value;
    }

    long int pid_t_start = 0.0f;
    if (value != "") { pid_t_start = long(std::stoi(value)) / sysconf(_SC_CLK_TCK); }

    return UpTime() - pid_t_start;
  }
  return 0;
}