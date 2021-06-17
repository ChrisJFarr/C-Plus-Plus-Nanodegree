#include "processor.h"

#include <fstream>
#include <string>

#include "linux_parser.h"
using std::fstream;
using std::string;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  string dump;  // Don't need this, it will return cpu
  int user, nice, system_int, idle, iowait, irq, softirq, steal, guest,
      guest_nice;
  string line;
  std::ifstream stream("/proc/stat");
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> dump >> user >> nice >> system_int >> idle >> iowait >> irq >>
        softirq >> steal >> guest >> guest_nice;
  }
  stream.close();

  // Compute utilization
  // Source:
  // https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
  int PrevIdle = prev_idle + prev_iowait;
  int Idle = idle + iowait;

  int PrevNonIdle = prev_user + prev_nice + prev_system_int + prev_irq +
                    prev_softirq + prev_steal;
  int NonIdle = user + nice + system_int + irq + softirq + steal;

  int PrevTotal = PrevIdle + PrevNonIdle;
  int Total = Idle + NonIdle;

  // differentiate: actual value minus the previous one
  int totald = Total - PrevTotal;
  int idled = Idle - PrevIdle;

  float CPU_Percentage = float(totald - idled) / float(totald);

  // Update prev's before returning
  prev_user = user;
  prev_nice = nice;
  prev_system_int = system_int;
  prev_idle = idle;
  prev_iowait = iowait;
  prev_irq = irq;
  prev_softirq = softirq;
  prev_steal = steal;
  prev_guest = guest;
  prev_guest_nice = guest_nice;

  return CPU_Percentage;
}
