#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  float prev_user = 0.;
  float prev_nice = 0.;
  float prev_system = 0.;
  float prev_irq = 0.;
  float prev_idle = 0.;
  float prev_iowait = 0.;
  float prev_softirq = 0.;
  float prev_steal = 0.;

  float PrevIdle = prev_idle + prev_iowait;
  float PrevNonIdle = prev_user + prev_nice + prev_system + prev_irq + prev_softirq + prev_steal;
  float PrevTotal = PrevIdle + PrevNonIdle;
};

#endif