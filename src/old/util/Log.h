#ifndef LOG_H
#define LOG_H
#include <string>
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#ifndef NO_DEBUG_OUTPUT
#define DEBUG(expression) Log::debug(expression);
#else
#define DEBUG(expression)
#endif

using namespace std;

class Log {
public:
  static bool debugMode;
  static void log(const string s);
  static void debug(const string s); 
  static void warn(const string s);
  static void error(const string s);
  static void success(const string s);
};
#endif
