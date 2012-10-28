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

int main(int argc, char* argv[]){
  cerr << "Initializing Tracker...\n";
  namedWindow("Display window", CV_WINDOW_AUTOSIZE);
  cerr << "Creating Theron Framework...\n";
  Theron::Framework framework;
  cerr << "Spawning Multimodal Actor...\n";
  MultimodalActor multimodalActor(framework);
  cerr << "Spawning Frame Analyzer Actor...\n";
  FrameAnalyzerActor frameAnalyzerActor(framework,multimodalActor.GetAddress());
  cerr <<"Spawning Georeferencing Actor...\n"; 
  GeoreferencingActor georeferencingActor(framework,32.0, -117.0, 0.0,multimodalActor.GetAddress());
  cerr <<"Spawning VideoReceiver Interface...\n"; 
  //VideoReceiverInterface videoInterface(framework,frameAnalyzerActor.GetAddress());
  cerr <<"Spawning GPSReceiver Interface...\n"; 
  GPSReceiverInterface gpsReceiverInterface(framework, georeferencingActor.GetAddress());
  cerr << "Tracker Initialization Complete.\n";
  cerr <<"Press any key to exit\n";
  getchar();
}
