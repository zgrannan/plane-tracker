#include "GeoReference.h"
#include <vector>
#include <cmath>
using namespace std;
using namespace GeoReference;

double GeoReference::degToRad(double deg) {
  return deg * PI / 180;
}

// Taken from: http://www.movable-type.co.uk/scripts/latlong.html

vector<double> GeoReference::calculateBearingAndDistance(double lat1, double lon1, double lat2, double lon2){
  double dLat = degToRad(lat2-lat1);
  double dLon = degToRad(lon2-lon1);
  double lat1r = degToRad(lat1);
  double lat2r = degToRad(lat2);
  double angle = pow(sin(dLat/2),2) + pow(sin(dLon/2),2) * cos(lat1r) * cos(lat2r);
  double c = 2 * atan2(sqrt(angle),sqrt(1-angle));
  double distance = RADIUS_M * c;
  vector<double> result;
  result.push_back(angle * 180 / PI);
  result.push_back(distance);
  return result;
}  
