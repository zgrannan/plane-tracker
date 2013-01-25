#ifndef GEOREFERENCE_H
#define GEOREFERENCE_H
#include <utility>
using std::pair;

namespace GeoReference{
  const double RADIUS_M = 6371000;
  const double PI = 3.14159;
  double degToRad(const double deg);
  std::pair<double,double> calculatePanAndTilt(
    const double lat1, const double lon1, const double alt1,
    const double lat2, const double lon2, const double alt2);
}

#endif
