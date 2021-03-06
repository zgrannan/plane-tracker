#include "src/gps/Protocol.h"
#include "src/util/Messages.h"
#include "src/util/Log.h"
#include <string>
#include <cstdlib>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

GPSDataMessage Protocol::parseSerialInputForGPS(const string input, string& extra) {
  vector<string> lines;
  boost::split(lines,input,is_any_of("\n"));
  for (unsigned int i = 0; i < lines.size(); i++){
    vector<string> chunks;
    boost::split(chunks,lines[i],is_any_of(","));
    const string lastChunk = chunks[chunks.size()-1];
    const char lastChar = lastChunk[lastChunk.length()-1];
    if (chunks[0] == "$GPGGA") {
      if (lastChar == '\r') {
        extra = "";
        return parseSerialInputLineForGPS(lines[i]);
      } else {
        DEBUG("Chunk " +  chunks[chunks.size() - 1]);
        extra = lines[i];
      }
    }
  }
  return GPSDataMessage();
}

GPSDataMessage Protocol::parseSerialInputLineForGPS(const string input) { 
  //$GPGGA,hhmmss.ss,Latitude,N,Longitude,E,FS,NoSV,HDOP,msl,m,Altref,m,DiffAge,DiffStation*cs<CR><LF> 
  double lat,lon,alt;
  vector<string> strs;
  boost::split(strs,input,is_any_of(","));
  if (strs.size() < 10 ) {
    Log::warn("Invalid message: " + input);
    return GPSDataMessage();
  }
  DEBUG("Received message: " +  input);
  if (strs[2] == "" || strs[3] == "" || strs[4] == "" || strs[5] == "") {
    DEBUG("GPS Data not yet available");
    return GPSDataMessage();
  }
  if (strs[3] == "N"){
    lat = parseLatitudeOrLongitude(strs[2]);
  } else {
    lat = 0 - parseLatitudeOrLongitude(strs[2]);
  }
  if(strs[5] == "E"){
    lon = parseLatitudeOrLongitude(strs[4]);
  } else {
    lon = 0 - parseLatitudeOrLongitude(strs[4]);
  }
  alt = atof(strs[9].c_str());
  Log::log("Recieved plane position: (" + boost::lexical_cast<string>(lat) + "," + boost::lexical_cast<string>(lon) + ") @ "+ boost::lexical_cast<string>(alt) + " m");
  return GPSDataMessage(lat,lon,alt);
}

double Protocol::parseLatitudeOrLongitude(const string input){
  DEBUG("Parsing " + input + " as lat or lon");
  const double raw = atof(input.c_str()) / 100;
  const int degrees = (int)(raw);
  const double minutes = (raw - degrees) * 100; 
  const double minutesFraction = minutes / 60.0;
  return (double)degrees + minutesFraction;
}
