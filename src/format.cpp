#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    int h = seconds / 3600;
    int m = (int) (seconds - h * 3600) / 60;
    int s = seconds - 3600 * h - 60 * m;

    std::string hour, min, sec;
    hour = std::to_string(h);
    min  = std::to_string(m);
    sec  = std::to_string(s);
    hour.insert(0,2 - hour.size(),'0');
    min.insert(0,2 - min.size(),'0');
    sec.insert(0,2 - sec.size(),'0');

    return hour + ":" + min + ":" + sec;
}