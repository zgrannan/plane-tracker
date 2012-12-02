#include "src/gps/GeoReference.h"
#include <cstdlib>
#include <iostream>
using namespace std;

int main(int argc, char** argv){
  if ( argc != 7 ){
    cout << "Usage: " << argv[0] << " lat1 lon1 alt1 lat2 lon2 alt2\n";
    return 0;
  }
  double lat1 = atof(argv[1]);
  double lon1 = atof(argv[2]);
  double alt1 = atof(argv[3]);
  double lat2 = atof(argv[4]);
  double lon2 = atof(argv[5]);
  double alt2 = atof(argv[6]);
  double roll, pitch;
  cout << GeoReference::calculateBearingAndDistance(lat1,lon1,lat2,lon2)[0] << endl;
  cout << GeoReference::calculateBearingAndDistance(lat1,lon1,lat2,lon2)[1] << endl;
}
