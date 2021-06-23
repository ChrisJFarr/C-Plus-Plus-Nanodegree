#include "process.h"
#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>

#include "linux_parser.h"
#include "system.h"

using std::string;
using std::to_string;
using std::vector;
using std::ifstream;

void Process::SetPid(int p){
  pid = p;
}

// DONE: Return this process's ID
int Process::Pid() {
  // proc/(int)
  return pid;
}

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() {

  // Declare support variables
  string line, token;
  float utime, stime, cutime, cstime, starttime;
  
  // Open file /proc/stat
  std::ifstream stream(LinuxParser::kProcDirectory + std::to_string(Pid()) + LinuxParser::kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
  }

  // Parse line
  std::istringstream line_stream(line);
  int i = 1;
  while(true){
    line_stream >> token;
    switch(i){
      case 14:
        try {
          utime = std::stof(token);
        } 
        catch(...) {
          utime = 0.00001;
        }
        break;
      case 15:
        try {
          stime = std::stof(token);
        } 
        catch(...) {
          stime = 0.00001;
        }
        break;
      case 16:
        try {
          cutime = std::stof(token);
        } 
        catch(...) {
          cutime = 0.00001;
        }
        break;
      case 17:
        try {
          cstime = std::stof(token);
        } 
        catch(...) {
          cstime = 0.00001;
        }
        break;
      case 22:
        try {
          starttime = std::stof(token);
        } 
        catch(...) {
          starttime = 0.00001;
        }
        break;
    }
    i++;
    if(i > 22){
      break;
    }
  }

  // Declare additional variables
  float total_time, seconds, cpu_usage, hertz;

  // Get hertz of system
  hertz = sysconf(_SC_CLK_TCK);

  // Compute utilization
  total_time = utime + stime + cutime + cstime;
  seconds = LinuxParser::UpTime() - (starttime / hertz);
  cpu_usage = (total_time / hertz) / seconds;

  // Set the private value for caching
  cpu_utilization = cpu_usage;

  return cpu_utilization;
}

// DONE: Return the command that generated this process
string Process::Command() {
  // proc/[pid]/cmdline

  // Declare support variables
  string line, cmd_string;

  // Open file /proc/[pid]/cmdline
  std::ifstream stream(LinuxParser::kProcDirectory + std::to_string(Pid()) + LinuxParser::kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
  }
  std::istringstream line_stream(line);
  line_stream >> cmd_string;

  return cmd_string;
}

// DONE: Return this process's memory utilization
string Process::Ram() {

  // Declare support variables
  float mem_utilization = 0;
  string line, token, mem_output;

  // Stream file contents
  std::ifstream stream(LinuxParser::kProcDirectory + 
                      std::to_string(Pid()) + 
                      LinuxParser::kStatusFilename);

  // Find line that starts with VmSize:
  int i = 30;
  while(stream.is_open())
  {
    // Get the next line
    std::getline(stream, line);
    // Parse the first token
    std::istringstream line_stream(line);
    line_stream >> token;
    if(token == "VmSize:"){
      // It's the correct line, get the next token
      line_stream >> token;
      mem_utilization = std::stof(token);
      // Convert to MB
      mem_utilization /= 1000;
      break;
    } else {
      i--;
      if(i < 0){
        // Allow a max of 30 lines parsed before deciding memory not available
        break;
      }
    }
  }

  // Manage precision
  // https://thispointer.com/c-convert-double-to-string-and-manage-precision-scientific-notation/
  std::ostringstream out_stream;
  out_stream << std::fixed;
  out_stream << std::setprecision(2);
  out_stream << mem_utilization;
  mem_output = out_stream.str();

  return mem_output;
}

// DONE: Return the user (name) that generated this process
string Process::User() {
  // proc/status
  // etc/passwd to map to user

    // Declare support variables
  string line, dump, token, uid, user;

  // Stream file contents
  std::ifstream stream1(LinuxParser::kProcDirectory + 
                      std::to_string(Pid()) + 
                      LinuxParser::kStatusFilename);

  // Find line that starts with goal token:
  int max_lines = 15;  // Max number of lines to parse
  while(stream1.is_open())
  {
    // Get the next line
    std::getline(stream1, line);
    // Parse the first token
    std::istringstream line_stream(line);
    line_stream >> token;
    if(token == "Uid:"){
      // It's the correct line, get the next token
      line_stream >> uid;
      break;
    } else {
      max_lines--;
      if(max_lines < 0){
        // Allow max_lines lines parsed before deciding not available
        break;
      }
    }
  }

  // Stream file contents
  std::ifstream stream2(LinuxParser::kPasswordPath);

  // Find line that starts with goal token:
  while(stream2.is_open())
  {
    // Get the next line
    std::getline(stream2, line);
    // Replace : with spaces
    std::replace(line.begin(), line.end(), ':', ' ');
    // Stream tokens and look for uid match
    std::istringstream line_stream(line);
    line_stream >> user >> dump >> token;
    if(token == uid){
      // It's the correct line and user has already been stored in variable
      break;
    }
  }
  return user;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
  // proc/[pid]/stat

  // Declare support variables
  string line, token;
  float start_time, up_time;

  // Stream file contents
  std::ifstream stream(LinuxParser::kProcDirectory + 
                      std::to_string(Pid()) + 
                      LinuxParser::kStatFilename);
  // Get the next line
  std::getline(stream, line);
  // Stream tokens
  std::istringstream line_stream(line);

  // Go to the target token number
  int target_token_number = 22;
  while(true)
  {
    // Get next token
    line_stream >> token;
    // Check for target
    if(target_token_number == 1){
      break;
    } else {
      target_token_number--;
    }
  }

  // Convert to float
  try {
    start_time = std::stof(token);
  } 
  catch(...) {
    start_time = 0.00001;
  }
  
  // Divide by clock ticks
  float hertz = sysconf(_SC_CLK_TCK);
  start_time /= hertz;

  // Subtract start time from current time
  up_time = LinuxParser::UpTime() - start_time;

  // Update the cached value
  cached_up_time = long(up_time);

  return long(up_time);
}

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return cpu_utilization > a.cpu_utilization;
}