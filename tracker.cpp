#include "VideoReceiverInterface.h"
#include "GPSReceiverInterface.h"
#include "FrameAnalyzerActor.h"
#include "GeoreferencingActor.h"
#include "MultiModalActor.h"
#include <Theron/Theron.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void usage(){
  cout <<"USAGE: tracker [lat lon alt][gps_serial_port arduino_serial_port]\n";
}

int main(int argc, char* argv[]){
  double lat = 32;
  double lon = -117;
  double alt = 0;
  string gpsSerial = "/dev/ttyw0";
  string arduinoSerial = "/dev/ttyw1";
  if (argc != 1 && argc != 4 && argc != 6 ){
    usage();
    return 0;
  } 
  if (argc >= 4){
    lat = atof(argv[1]); 
    lon = atof(argv[2]);
    alt = atof(argv[3]); 
  }
  if (argc == 6 ){
    gpsSerial = string(argv[4]);
    arduinoSerial = string(argv[5]);
  }
  cerr << "Initializing Tracker...\n";
  namedWindow("Display window", CV_WINDOW_AUTOSIZE);
  cerr << "Creating Theron Framework...\n";
  Theron::Framework framework;
  cerr << "Spawning Multimodal Actor...\n";
  MultimodalActor multimodalActor(framework,arduinoSerial);
  cerr << "Spawning Frame Analyzer Actor...\n";
  FrameAnalyzerActor frameAnalyzerActor(framework,multimodalActor.GetAddress());
  cerr <<"Spawning Georeferencing Actor...\n"; 
  GeoreferencingActor georeferencingActor(framework,lat,lon,alt,multimodalActor.GetAddress());
  cerr <<"Spawning VideoReceiver Interface...\n"; 
  VideoReceiverInterface videoInterface(framework,frameAnalyzerActor.GetAddress());
  cerr <<"Spawning GPSReceiver Interface...\n"; 
  GPSReceiverInterface gpsReceiverInterface(framework, gpsSerial, georeferencingActor.GetAddress());
  cerr << "Tracker Initialization Complete.\n";
  cerr <<"Press any key to exit\n";
  getchar();
}
