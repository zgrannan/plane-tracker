#include "src/gps/GeoReference.h"
#include <vector>
#include <cmath>
#include <iostream>
using namespace std;
using namespace GeoReference;

double GeoReference::degToRad(const double deg) {
  return deg * PI / 180.0;
}

// Taken from: http://www.movable-type.co.uk/scripts/latlong.html

pair<double,double> GeoReference::calculatePanAndTilt(const double lat1, 
                                                      const double lon1,
                                                      const double alt1,
                                                      const double lat2,
                                                      const double lon2,
                                                      const double alt2){
  const double dy = (lat2 - lat1) * M_PI / 180;
  const double dx = (cos(M_PI/180*lat1)*(lon2 - lon1) * M_PI / 180);
  const double angle = atan2(dy, dx);
  const double x = M_PI / 180 * (lon2-lon1) * cos(((M_PI/360) *(lat1+lat2)));
  const double y = M_PI / 180 * (lat2-lat1);
  const double distance = sqrt(x*x+y*y) * RADIUS_M;
  const double dAltitude = fabs(alt2-alt1);
  const double pan = (-angle * 180.0 / M_PI);
  const double tilt = tan(dAltitude/distance) * 180.0 / M_PI;
  return {tilt,pan};
}  

