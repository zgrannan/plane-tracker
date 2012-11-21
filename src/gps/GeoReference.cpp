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

vector<double> GeoReference::calculateBearingAndDistance(double lat1, double lon1, double lat2, double lon2){
  double dy = (lat2 - lat1);
  double dx = (cos(M_PI/180*lat1)*(lon2 - lon1));
  double angle = atan2(dy, dx);
  double c = 2.0 * atan2(sqrt(angle),sqrt(1.0-angle));
  double distance = RADIUS_M * c;
  vector<double> result;
  result.push_back(-angle * 180.0 / PI);
  result.push_back(distance);
  return result;
}  
