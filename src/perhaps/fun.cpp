#include "perhaps.h"
#include "fun.h"
Option<double> perhapsFun::safeDivide(double top, double bottom) {
  if (bottom != 0 ){
    return Some<double>(top/bottom);
  } else {
    return None<double>();
  }
}
