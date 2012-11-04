#include "Log.h"
#include <iostream>

using namespace std;

bool Log::debugMode = false;

void Log::log(string s) {
  cout << KNRM <<  s << endl;
}

void Log::error(string s) {
  cerr << KRED << s << endl;
}

void Log::warn(string s) {
  cerr << KYEL << s << endl;
}

void Log::debug(string s) {
  if (debugMode)
    cout << KBLU << s << endl;
}

void Log::success(string s) {
  cout << KGRN << s << endl;
}
