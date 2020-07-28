#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  // constructor
  Process(){};
  Process(int pid);

  // accessors
  int Pid();                               
  std::string User();                      
  std::string Command();                   
  float CpuUtilization();                  
  std::string Ram() const;                       
  long int UpTime();                       
  bool operator<(Process const& other) const;  

  // mutators
  void Pid(int p);
  void User(std::string u);
  void Ram(std::string r);
  void Command(std::string c);
  void UpTime(long int ut);

 private:
  int m_pid;
  std::string m_user;
  std::string m_ram;
  std::string m_cmd;
  long int m_uptime;
};

#endif