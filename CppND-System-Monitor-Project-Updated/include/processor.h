#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  int read_utilization();
  // TODO: Declare any necessary private members
 private:
  int prev_user, prev_nice, prev_system_int, prev_idle, prev_iowait, prev_irq,
      prev_softirq, prev_steal, prev_guest, prev_guest_nice;
};

#endif