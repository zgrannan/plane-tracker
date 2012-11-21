#include "../gps/GeoReference.h"
#include <cstdlib>
#include <iostream>
using namespace std;

int main(int argc, char** argv){
  double lat1 = atof(argv[1]);
  double lon1 = atof(argv[2]);
  double lat2 = atof(argv[3]);
  double lon2 = atof(argv[4]);
  cout << lat1 << "\n" << lon1 << "\n" << lat2 << "\n" << lon2 << endl;
  cout << GeoReference::calculateBearingAndDistance(lat1,lon1,lat2,lon2)[0] << endl;
}
