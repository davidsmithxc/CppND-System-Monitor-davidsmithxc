#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Done: Process Constructor
Process::Process(int pid) {
  Pid(pid);
  User(LinuxParser::User(pid));
  // Ram(LinuxParser::Ram(pid));
  Command(LinuxParser::Command(pid));
  UpTime(LinuxParser::UpTime(pid));
}

// *** Mutators ***
void Process::Pid(int p) { m_pid = p; }

void Process::User(std::string u) { m_user = u; }

void Process::Ram(std::string r) { m_ram = r; }

void Process::Command(std::string c) { m_cmd = c; }

void Process::UpTime(long int ut) { m_uptime = ut; }

// *** Accessors ***

// DONE: Return this process's ID
int Process::Pid() { return m_pid; }

// Done: Return this process's CPU utilization
float Process::CpuUtilization() {
  long int active_pid_time =
      LinuxParser::ActiveJiffies(m_pid) / sysconf(_SC_CLK_TCK);

  return (float(active_pid_time) / float(LinuxParser::UpTime()));
}

// DONE: Return the command that generated this process
string Process::Command() { return m_cmd; }

// DONE: Return this process's memory utilization
string Process::Ram() const { return LinuxParser::Ram(m_pid); }

// DONE: Return the user (name) that generated this process
string Process::User() { return m_user; }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return m_uptime; }

// Done: Overload the "less than" comparison operator for Process objects
// TODO: Add sorting for something other than ram
bool Process::operator<(Process const& other) const {
  float other_val = std::stof(other.Ram());
  float this_val = std::stof(Ram());
  return this_val > other_val;
}