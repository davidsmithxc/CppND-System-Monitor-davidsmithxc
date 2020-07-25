#include "format.h"

#include <string>
#include <vector>

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  std::vector<std::string> time_str(3);
  std::vector<long int> time_l(3);

  // integer division, assume truncating toward zero
  time_l[0] = seconds / 3600;
  seconds -= time_l[0] * 3600;

  time_l[1] = seconds / 60;
  seconds -= time_l[1] * 60;

  time_l[2] = seconds;

  for (int i = 0; i < 3; i++) {
    time_str[i] = std::to_string(time_l[i]);
    if (time_str[i].size() == 1) time_str[i] = "0" + time_str[i];
  }

  return time_str[0] + ":" + time_str[1] + ":" + time_str[2];
}