#include <Protocol.h>
#include <Messages.h>
#include <string>
#include <cstdlib>
#include <boost>

using namespace std;
using namespace boost;
GPSDataMessage Protocol::parseSerialInputForGPS(string input){
  // Message Structure:
  // $PUBX,00,hhmmss.ss,Latitude,N,Longitude,E,AltRef,NavStat,Hacc,Vacc,SOG,COG,Vvel,ageC,HDOP,VDOP,TDOP, GU,RU,DR,*cs<CR><LF>
  double lat,lon,alt;
  vector<string> strs;
  boost::split(strs,input,is_any_of(","));
  lat = parseLatitudeOrLongitude(strs[3]);
  if(strs[4] == 'N'){
    lon = parseLatitudeOrLongitude(strs[5]);
  } else {
    lon = 0 - parseLatitudeOrLongitude(strs[5]);
  }
  alt = atof(strs[7]);
  return GPSDataMessage(lat,lon,alt);
}

double Protocol::parseLatitudeOrLongitude(string input){
  double raw = atof(input) / 100;
  int degrees = (int)(raw);
  double minutes = (raw - degrees) * 100; 
  double minutesFraction = minutes / 60.0;
  return (double)degrees + minutesFraction;
}
