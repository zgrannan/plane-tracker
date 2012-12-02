#include "src/gps/GeoReference.h"
#include <vector>
#include <cmath>
#include <iostream>
using namespace std;
using namespace GeoReference;

double GeoReference::degToRad(double deg) {
  return deg * PI / 180.0;
}

// Taken from: http://www.movable-type.co.uk/scripts/latlong.html

pair<double,double> GeoReference::calculatePanAndTilt(double lat1, double lon1, double alt1,
                                                      double lat2, double lon2, double alt2){
  double dy = (lat2 - lat1) * M_PI / 180;
  double dx = (cos(M_PI/180*lat1)*(lon2 - lon1) * M_PI / 180);
  double angle = atan2(dy, dx);
  double x = M_PI / 180 * (lon2-lon1) * cos(((M_PI/360) *(lat1+lat2)));
  double y = M_PI / 180 * (lat2-lat1);
  double distance = sqrt(x*x+y*y) * RADIUS_M;
  double dAltitude = fabs(alt2-alt1);
  double pan = (-angle * 180.0 / M_PI);
  double tilt = tan(dAltitude/distance) * 180.0 / M_PI;
  return {tilt,pan};
}  

