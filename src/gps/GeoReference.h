#ifndef GEOREFERENCE_H
#define GEOREFERENCE_H
#include <utility>
using std::pair;

namespace GeoReference{
  const double RADIUS_M = 6371000;
  const double PI = 3.14159;
  double degToRad(double deg);
  std::pair<double,double> calculatePanAndTilt(
    double lat1, double lon1, double alt1,
    double lat2, double lon2, double alt2);
}

#endif
