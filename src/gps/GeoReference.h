#ifndef GEOREFERENCE_H
#define GEOREFERENCE_H
#include <vector>
using namespace std;

namespace GeoReference{
  const double RADIUS_M = 6371000;
  const double PI = 3.14159;
  double degToRad(double deg);
  vector<double> calculateBearingAndDistance(double lat1, double lon1, double lat2, double lon2);
}

#endif
