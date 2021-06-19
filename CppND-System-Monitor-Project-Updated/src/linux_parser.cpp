#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  // /proc/meminfo
  // Compute total used memory and divide by total memory
  // (MemTotal - MemFree) / MemTotal
  // Declare support variables
  float mem_utilization;
  int mem_total, mem_free;
  string line, dump;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  // Get mem_total
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream line_1_stream(line);
    line_1_stream >> dump >> mem_total >> dump;
    // Get mem_free
    std::getline(stream, line);
    std::istringstream line_2_stream(line);
    line_2_stream >> dump >> mem_free >> dump;
    // Compute memory utilization
    mem_utilization = float(mem_total - mem_free) / float(mem_total);
  }
  return mem_utilization;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  // Declare support variables
  string line;
  long int active_time, suspend_time;
  std::ifstream stream(kProcDirectory + kUptimeFilename);

  if (stream.is_open()) {
    // Read first line (only line)
    std::getline(stream, line);

    // Parse line
    std::istringstream line_stream(line);
    line_stream >> active_time >> suspend_time;
  }

  return active_time + suspend_time;  // TODO Evaluate: should I add suspend time?
}

// // TODO: Read and return the number of jiffies for the system
// long LinuxParser::Jiffies() { return 0; }

// // TODO: Read and return the number of active jiffies for a PID
// // REMOVE: [[maybe_unused]] once you define the function
// long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// // TODO: Read and return the number of active jiffies for the system
// long LinuxParser::ActiveJiffies() { return 0; }

// // TODO: Read and return the number of idle jiffies for the system
// long LinuxParser::IdleJiffies() { return 0; }

// // NEEDED?: Read and return CPU utilization
// vector<string> LinuxParser::CpuUtilization() { return {}; }

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {

  // Declare support variables
  string line, line_head;
  int total_processes;

  // Open file /proc/meminfo
  std::ifstream stream(kProcDirectory + kStatFilename);

  // Loop through lines until line starts with "processes"
  while (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream line_stream(line);
    line_stream >> line_head;
    if (line_head == "processes") break;
  }

  // Parse line
  std::istringstream line_stream(line);
  line_stream >> line_head >> total_processes;

  return total_processes;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {

  // Declare support variables
  string line, line_head;
  int running_processes;

  // Open file /proc/meminfo
  std::ifstream stream(kProcDirectory + kStatFilename);

  // Loop through lines until line starts with "processes"
  while (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream line_stream(line);
    line_stream >> line_head;
    if (line_head == "procs_running") break;
  }

  // Parse line
  std::istringstream line_stream(line);
  line_stream >> line_head >> running_processes;

  return running_processes;
}

// // TODO: Read and return the command associated with a process
// // REMOVE: [[maybe_unused]] once you define the function
// string LinuxParser::Command(int pid [[maybe_unused]]) { return string(); }

// // TODO: Read and return the memory used by a process
// // REMOVE: [[maybe_unused]] once you define the function
// string LinuxParser::Ram(int pid [[maybe_unused]]) { 
//   return string(); 
//   }

// // TODO: Read and return the user ID associated with a process
// // REMOVE: [[maybe_unused]] once you define the function
// string LinuxParser::Uid(int pid [[maybe_unused]]) { return string(); }

// // TODO: Read and return the user associated with a process
// // REMOVE: [[maybe_unused]] once you define the function
// string LinuxParser::User(int pid [[maybe_unused]]) { return string(); }

// // TODO: Read and return the uptime of a process
// // REMOVE: [[maybe_unused]] once you define the function
// long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }
