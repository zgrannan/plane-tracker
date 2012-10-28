#include "Protocol.h"
#include "Messages.h"
#include <string>
#include <cstdlib>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;
GPSDataMessage Protocol::parseSerialInputForGPS(string input){
  // Message Structure:
  // $PUBX,00,hhmmss.ss,Latitude,N,Longitude,E,AltRef,NavStat,Hacc,Vacc,SOG,COG,Vvel,ageC,HDOP,VDOP,TDOP, GU,RU,DR,*cs<CR><LF>
  double lat,lon,alt;
  vector<string> strs;
  boost::split(strs,input,is_any_of(","));
  if (strs.size() != 22){
    throw string("Invalid serial data received: " + input);
  }
  lat = parseLatitudeOrLongitude(strs[3]);
  if(strs[4] == "E"){
    lon = parseLatitudeOrLongitude(strs[5]);
  } else {
    lon = 0 - parseLatitudeOrLongitude(strs[5]);
  }
  alt = atof(strs[7].c_str());
  cout <<"Recieved plane position: ("<<lat<<","<<lon<<") @ "<<alt<<" m\n";
  return GPSDataMessage(lat,lon,alt);
}

double Protocol::parseLatitudeOrLongitude(string input){
  cerr <<"Parsing "<<input<<" as lat or lon\n";
  double raw = atof(input.c_str()) / 100;
  int degrees = (int)(raw);
  double minutes = (raw - degrees) * 100; 
  double minutesFraction = minutes / 60.0;
  return (double)degrees + minutesFraction;
}
