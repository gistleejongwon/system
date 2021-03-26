#include "processor.h"
#include "linux_parser.h"
#include <fstream>
#include <string>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if (filestream.is_open()){
    std::string line;
    std::getline(filestream,line);
    std::istringstream sstr(line);
    std::string ignore;
    float user, nice, system, irq, idle, iowait, softirq, steal;
    
    sstr >> ignore >> user >> nice >> system >> irq >> idle >> iowait >> softirq >> steal;
    
    /*
    PrevIdle = previdle + previowait
    Idle = idle + iowait

    PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal
    NonIdle = user + nice + system + irq + softirq + steal

    PrevTotal = PrevIdle + PrevNonIdle
    Total = Idle + NonIdle

    # differentiate: actual value minus the previous one
    totald = Total - PrevTotal
    idled = Idle - PrevIdle

    CPU_Percentage = (totald - idled)/totald
    */
    float Idle = idle + iowait;
    float NonIdle = user + nice + system +irq + softirq + steal;
    float Total = Idle + NonIdle;

    float totald = Total - PrevTotal;
    float idled = Idle - PrevIdle;

    prev_user = user;
    prev_nice = nice;
    prev_system = system;
    prev_irq = idle;
    prev_iowait = iowait;
    prev_softirq = softirq;
    prev_steal = steal;
    
    return (totald - idled)/totald;
  }
  return 0.;
}