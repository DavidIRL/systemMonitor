#include "format.h"

#include <string>
using std::string;

string Format::ElapsedTime(long seconds) {
  string hr = static_cast<string>(static_cast<int>(seconds / 3600));
  string min = static_cast<string>(static_cast<int>((seconds % 3600) / 60));
  string sec = static_cast<string>(static_cast<int>((seconds % 3600) % 60));
  // prepend 0s as needed for HH:MM:SS
  string hrs = string(2 - hr.length(), "0") + hr;
  string mins = string(2 - min.length(), "0") + min;
  string secs = string(2 - sec.length(), "0") + sec;
  string result = "";

  if (hrs.compare("00") != 0) {
    result = hrs + ":" + mins + ":" + secs;
  } else {
    result = mins + ":" + secs;
  }
  return result;
}
