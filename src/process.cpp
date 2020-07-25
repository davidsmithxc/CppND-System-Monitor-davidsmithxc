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

// TODO: Process Constructor
Process::Process(int pid) {
  Pid(pid);
  User(LinuxParser::User(pid));
  Ram(LinuxParser::Ram(pid));
  Command(LinuxParser::Command(pid));
}

// *** Mutators ***
void Process::Pid(int p) { m_pid = p; }

void Process::User(std::string u) { m_user = u; }

void Process::Ram(std::string r) { m_ram = r; }

void Process::Command(std::string c) { m_cmd = c; }

// *** Accessors ***

// TODO: Return this process's ID
int Process::Pid() { return m_pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return 0; }

// TODO: Return the command that generated this process
string Process::Command() { return m_cmd; }

// TODO: Return this process's memory utilization
string Process::Ram() { return m_ram; }

// TODO: Return the user (name) that generated this process
string Process::User() { return m_user; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return 0; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a [[maybe_unused]]) const {
  return true;
}