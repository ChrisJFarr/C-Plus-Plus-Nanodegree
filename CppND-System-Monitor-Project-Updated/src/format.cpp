#include "format.h"

#include <cstring>
#include <string>

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  int const N_ZEROS = 2;
  string hours, minutes, seconds_remaining;
  // TODO Fix this logic
  hours = std::to_string(seconds / 3600);
  minutes = std::to_string((seconds % 3600) / 60);
  seconds_remaining = std::to_string((seconds % 3600) % 60);
  // Pad output with zeros
  string output = std::string(N_ZEROS - hours.length(), '0') + hours + ":";
  output =
      output + std::string(N_ZEROS - minutes.length(), '0') + minutes + ":";
  output = output + std::string(N_ZEROS - seconds_remaining.length(), '0') +
           seconds_remaining;

  return output;
}