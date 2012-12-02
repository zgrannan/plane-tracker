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
  double dy = (lat2 - lat1) * M_PI / 180;
  double dx = (cos(M_PI/180*lat1)*(lon2 - lon1) * M_PI / 180);
  double angle = atan2(dy, dx);
  double x = M_PI / 180 * (lon2-lon1) * cos(((M_PI/360) *(lat1+lat2)));
  double y = M_PI / 180 * (lat2-lat1);
  double distance = sqrt(x*x+y*y) * RADIUS_M;
  vector<double> result;
  result.push_back(-angle * 180 / M_PI);
  result.push_back(distance);
  return result;
}  
