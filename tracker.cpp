#include "VideoReceiverInterface.h"
#include "GPSReceiverInterface.h"
#include "FrameAnalyzerActor.h"
#include "GeoreferencingActor.h"
#include "MultiModalActor.h"
#include "Vision.h"
#include <Theron/Theron.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <boost/program_options.hpp>

using namespace cv;
using namespace std;
using namespace Messages;

namespace po = boost::program_options;

/**
 * Displays the image on the screen for debugging purposes
 */
void showImage(string name, IplImage* image, float scale){
  CvSize newSize = cvSize((int)(image->width * scale),(int)(image->height * scale));
  IplImage* newImage = cvCreateImage(newSize,image->depth,image->nChannels);
  cvResize(image,newImage); 
  imshow(name,Mat(newImage));
  cvReleaseImage(&newImage);
}

int main(int argc, char* argv[]){
  struct arguments{
    double scale; 
    double lat; 
    double lon; 
    double alt; 
    string gpsSerial; 
    string arduinoSerial; 
    string videoFilename;
    string recordDirectory;
    bool showExtras;
    bool drawLine;
    bool blind;
  } arguments;

  double aDouble;
  string aString;
  po::options_description desc("Allowed options");
  desc.add_options()
    ("extras","Display intermediate steps")
    ("line", "Draw a line to the plane")
    ("record",po::value<string>(&aString)->default_value(""),"Display intermediate steps")
    ("lat",po::value<double>(&aDouble)->default_value(-32.0),"Set tracker latitude")
    ("lon",po::value<double>(&aDouble)->default_value(117.0),"Set tracker longitude")
    ("alt",po::value<double>(&aDouble)->default_value(0.0),"Set tracker altitude")
    ("scale",po::value<double>(&aDouble)->default_value(0.25),"Video scale")
    ("gps",po::value<string>(&aString)->default_value("/dev/tty0"),"Specify GPS serial port")
    ("arduino",po::value<string>(&aString)->default_value("/dev/tty1"),"Specify arduino serial port")
    ("video",po::value<string>(&aString)->default_value(""),"Simulate using video")
    ("blind","Do not use visual input");

  po::variables_map vm;
  po::store(po::command_line_parser(argc,argv).options(desc).run(),vm);
  po::notify(vm);

  arguments.showExtras = vm.count("extras");
  arguments.drawLine = vm.count("drawLine");
  arguments.blind = vm.count("blind");
  arguments.lat = vm["lat"].as<double>();
  arguments.lon = vm["lon"].as<double>();
  arguments.alt = vm["alt"].as<double>();
  arguments.scale = vm["scale"].as<double>();
  arguments.gpsSerial = vm["gps"].as<string>();
  arguments.arduinoSerial = vm["arduino"].as<string>();
  arguments.videoFilename = vm["video"].as<string>();
  arguments.recordDirectory = vm["record"].as<string>();

  /* Initialize variables */
  Theron::Receiver                          imageReceiver;
  Theron::Catcher<PlaneVisionMessage>       imageCatcher;
  Theron::Address                           from;
  PlaneVisionMessage                        message;
  Vision                                    *vision;

  cout << "Initializing tracker with the following settings: \n";
  cout << "Tracker Latitude: " << arguments.lat << endl;
  cout << "Tracker Longitude: " << arguments.lon << endl;
  cout << "Tracker Altitude: " << arguments.alt << endl;
  cout << "GPS Serial Device: " << arguments.gpsSerial << endl;
  cout << "Arduino Serial Device: " << arguments.arduinoSerial << endl;

  if (arguments.videoFilename == ""){
    cout << "Using live video stream\n";
  } else {
    cout << "Simulating flight using "<< arguments.videoFilename << endl;
  }

  if (arguments.showExtras) {
    cout << "Displaying intermediate steps\n";
    vision = new Vision(true);
  } else {
    vision = new Vision(false);
  }

  if (arguments.recordDirectory != "") {
    cout << "Recording output to: "<< arguments.recordDirectory << endl;
  }

  imageReceiver.RegisterHandler(&imageCatcher,&Theron::Catcher<PlaneVisionMessage>::Push);

  cout << "Creating Theron Framework...\n";
  Theron::Framework framework;

  cout << "Spawning Multimodal Actor...\n";
  MultimodalActor multimodalActor(framework,arguments.arduinoSerial);

  cout << "Spawning Frame Analyzer Actor...\n";
  FrameAnalyzerActor frameAnalyzerActor(framework, arguments.drawLine, vision, imageReceiver.GetAddress(), multimodalActor.GetAddress());

  cout <<"Spawning Georeferencing Actor...\n"; 
  GeoreferencingActor georeferencingActor(framework,arguments.lat,arguments.lon,arguments.alt,multimodalActor.GetAddress());

  if (!arguments.blind){
    cout <<"Spawning VideoReceiver Interface...\n"; 
    if (arguments.videoFilename == ""){
      new VideoReceiverInterface(framework,frameAnalyzerActor.GetAddress());
    } else {
      new VideoReceiverInterface(framework, arguments.videoFilename, frameAnalyzerActor.GetAddress());
    }
  } else {
    cout << "Not using video input\n";
  }

  cout <<"Spawning GPSReceiver Interface...\n"; 
  GPSReceiverInterface gpsReceiverInterface(framework, arguments.gpsSerial, georeferencingActor.GetAddress());

  cout << "Tracker Initialization Complete.\n";

  while (true){
    imageReceiver.Wait();
    imageReceiver.Consume(UINT_MAX);
    while (!imageCatcher.Empty()){
      imageCatcher.Pop(message,from);
    }
    for (int i = 0; i < message.extras.size(); i++){
      //showImage(message.extras[i].name,message.extras[i].image, arguments.scale);
      cvReleaseImage(&message.extras[i].image);
    }
    showImage("Display window", message.result, arguments.scale);
    cvReleaseImage(&message.result);
    cvWaitKey(1);
  }
}
