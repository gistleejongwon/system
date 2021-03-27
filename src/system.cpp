#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>


#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"
#include "format.h"

#include <fstream>
#include <sstream>
#include <algorithm>

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_;}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
  processes_.clear();
  vector<int> pids = LinuxParser::Pids();
  for(int pid : pids){
    Process process;
    process.Pid(pid);
    process.CpuUtilization();
    processes_.emplace_back(process);
  }
  std::sort(processes_.begin(),processes_.end());
  std::reverse(processes_.begin(),processes_.end());
  return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { 
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kVersionFilename);
  std::string line;
  std::string ig1,ig2, version;
  if(filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream ist(line);
    ist >> ig1 >> ig2 >> version;
  }
  return version;
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { 
  string path = LinuxParser::kProcDirectory + LinuxParser::kMeminfoFilename;
  float mem_total = LinuxParser::find_value(path,"MemTotal:");
  float mem_free = LinuxParser::find_value(path,"MemFree:");
  return (mem_total - mem_free) / mem_total;
}


// TODO: Return the operating system name
std::string System::OperatingSystem() { 
    return LinuxParser::OperatingSystem();
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() {
  string path = LinuxParser::kProcDirectory + LinuxParser::kStatFilename;
  return LinuxParser::find_value(path,"procs_running");
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { 
  string path = LinuxParser::kProcDirectory + LinuxParser::kStatFilename;
  return LinuxParser::find_value(path,"processes");
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime(){
  string path = LinuxParser::kProcDirectory + LinuxParser::kUptimeFilename;
  return LinuxParser::pick_value(path,0);
}