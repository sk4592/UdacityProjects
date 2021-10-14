#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    string final{};
    long H{0}, M{0}, S{0};
    H = seconds / 3600;
    M = (seconds % 3600) / 60;
    S = (seconds % 3600) % 60;
    
    string h = std::to_string(H);
    string m = std::to_string(M);
    string s = std::to_string(S);
    if (H < 10) { h = "0" + h; }
    if (M < 10) { m = "0" + m; }
    if (S < 10) { s = "0" + s; }
    return h + ":" + m + ":" + s;
}