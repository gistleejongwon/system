#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <typeinfo>

#include "process.h"
#include "linux_parser.h"
#include "system.h"
#include "format.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() const{ return Pid_;}

void Process::Pid(int pid){ 
    Pid_ = pid;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization(){
    string path = LinuxParser::kProcDirectory + std::to_string(Process::Pid()) + LinuxParser::kStatFilename;
    float utime = LinuxParser::pick_value(path,14);
    float stime = LinuxParser::pick_value(path,15);
    float cutime = LinuxParser::pick_value(path,16);
    float cstime = LinuxParser::pick_value(path,17);

    float Hertz = sysconf(_SC_CLK_TCK);

    float total_time = utime + stime + cutime + cstime;
    total_time = total_time / Hertz;
    float cpu_usage = total_time / Process::UpTime();
    Cpu_ = cpu_usage;

    return Cpu_;
}

// TODO: Return the command that generated this process
string Process::Command() { 
    string path = LinuxParser::kProcDirectory + std::to_string(Process::Pid()) + LinuxParser::kCmdlineFilename;
    std::ifstream filestream(path);
    string line;
    if(filestream.is_open()){
      getline(filestream,line);
    }
    return line;
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
    string path = LinuxParser::kProcDirectory + std::to_string(Process::Pid()) + LinuxParser::kStatusFilename;
    float ram_usage = LinuxParser::find_value(path, "VmSize:");
    string str_ram = std::to_string((int) (ram_usage * 0.001));
    str_ram.insert(0,5 - str_ram.size(),' ');
    return str_ram + "MB";
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
    //find user id.
    string path = LinuxParser::kProcDirectory + std::to_string(Process::Pid()) + LinuxParser::kStatusFilename;
    int Uid = LinuxParser::find_value(path,"Uid:");
    string path2 = LinuxParser::kPasswordPath;
    std::ifstream filestream(path2);

    if(filestream.is_open()){
        string line;
        string key;
        string ig;
        string value;
        vector<string> str_vec;
        
        while (!filestream.eof()){
            getline(filestream,line);
            std::istringstream sstr(line);
            string token;

            while(getline(sstr,token,':')){
              str_vec.push_back(token);
            }

            if (str_vec[2] == to_string(Uid))
              return str_vec[0];
             
        }
    }
  return string();
}


// TODO: Return the age of this process (in seconds)
long int Process::UpTime() const {
    string path = LinuxParser::kProcDirectory + std::to_string(Process::Pid()) + LinuxParser::kStatFilename;
    long int time = LinuxParser::pick_value(path, 22);
    System system;
    long int seconds = system.UpTime() - (time / sysconf(_SC_CLK_TCK));
    return seconds;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process& another){ 
    return Cpu_ < another.Cpu_;
}