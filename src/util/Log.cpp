#include "src/util/Log.h"
#include <iostream>

using namespace std;

bool Log::debugMode = false;

void Log::log(string s) {
  cout << KNRM <<  s << KNRM << endl;
}

void Log::error(string s) {
  cerr << KRED << s << KNRM << endl;
}

void Log::warn(string s) {
  cerr << KYEL << s << KNRM << endl;
}

void Log::debug(string s) {
  if (debugMode)
    cout << KBLU << s << KNRM << endl;
}

void Log::success(string s) {
  cout << KGRN << s << KNRM << endl;
}
