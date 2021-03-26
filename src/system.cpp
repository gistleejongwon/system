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
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kMeminfoFilename);
  std::string line1, line2;
  std::string ig, mem_total_str, mem_free_str;
  float mem_total, mem_free;
  if(filestream.is_open()){
      std::getline(filestream, line1);
      std::getline(filestream, line2);
      std::istringstream sstr1(line1);
      std::istringstream sstr2(line2);
      sstr1 >> ig >> mem_total_str;
      sstr2 >> ig >> mem_free_str;
      
      mem_total = stof(mem_total_str);
      mem_free =  stof(mem_free_str);

      return (mem_total - mem_free) / mem_total;
  }
  return 0.;
}


// TODO: Return the operating system name
std::string System::OperatingSystem() { 
    return LinuxParser::OperatingSystem();
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() {
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if(filestream.is_open()){
    std::string line;
    std::string key;
    int value;
    while(!filestream.eof()){
      std::getline(filestream,line);
      std::istringstream sstr(line);
      while(sstr >> key >> value){
        if (key == "procs_running"){
          return value;
        }
      }
    }
  }
  return 0;
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { 
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if(filestream.is_open()){
    std::string line;
    std::string key;
    int value;
    while(!filestream.eof()){
      std::getline(filestream,line);
      std::istringstream sstr(line);
      while(sstr >> key >> value){
        if (key == "processes"){
          return value;
        }
      }
    }
  }
  return 0;
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime(){
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kUptimeFilename);
  if (filestream.is_open()){
    std::string line;
    std::getline(filestream,line);
    std::istringstream sstr(line);
    float time;
    sstr >> time;
    return (long int) time;
  }
  return 0;
}