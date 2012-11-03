#include "Protocol.h"
#include "Messages.h"
#include <string>
#include <cstdlib>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

GPSDataMessage Protocol::parseSerialInputForGPS(string input, string& extra) {
  vector<string> lines;
  boost::split(lines,input,is_any_of("\n"));
  for (int i = 0; i < lines.size(); i++){
    vector<string> chunks;
    boost::split(chunks,lines[i],is_any_of(","));
    string lastChunk = chunks[chunks.size()-1];
    char lastChar = lastChunk[lastChunk.length()-1];
    if (chunks[0] == "$GPGGA") {
      if (lastChar == '\r') {
        extra = "";
        return parseSerialInputLineForGPS(lines[i]);
      } else {
        cout << "Chunk "<< chunks[chunks.size() - 1] << endl;
        extra = lines[i];
      }
    }
  }
  return GPSDataMessage();
}

GPSDataMessage Protocol::parseSerialInputLineForGPS(string input) { 
  //$GPGGA,hhmmss.ss,Latitude,N,Longitude,E,FS,NoSV,HDOP,msl,m,Altref,m,DiffAge,DiffStation*cs<CR><LF> 
  double lat,lon,alt;
  vector<string> strs;
  boost::split(strs,input,is_any_of(","));
  if (strs.size() < 10 ) {
    cout << "Invalid message: " << input << endl;
    return GPSDataMessage();
  }
  cout << "Received message: " << input << endl;
  if (strs[2] == "" || strs[3] == "" || strs[4] == "" || strs[5] == "") {
    cout << "GPS Data not yet available\n";
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
