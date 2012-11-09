#include "VideoReceiverInterface.h"
#include "GPSReceiverInterface.h"
#include "FrameAnalyzerActor.h"
#include "GeoreferencingActor.h"
#include "MultiModalActor.h"
#include "Vision.h"
#include "Log.h"
#include "ui.h"
#include <Theron/Theron.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <thread>
#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <QtGui/QApplication>

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

int baudRate(string baudRate){
  if (baudRate == "4800") return B4800;
  if (baudRate == "9600") return B9600;
  if (baudRate == "19200") return B19200;
  if (baudRate == "38400") return B38400;
  if (baudRate == "57600") return B57600;
  if (baudRate == "76800") return B76800;
  if (baudRate == "115200") return B115200;
  Log::error("Invalid baudrate: " + baudRate);
  exit(1);
  return -1;
}

int main(int argc, char* argv[]){
  QApplication a(argc,argv);
  UI ui;
  ui.show();
  struct arguments{
    double scale; 
    double lat; 
    double lon; 
    double alt; 
    int gpsBaud;
    int arduinoBaud;
    string gpsPort; 
    string arduinoPort; 
    string videoFilename;
    string recordDirectory;
    string imageFilename;
    bool showExtras;
    bool drawLine;
    bool blind;
    bool noGPS;
    bool debug;
  } arguments;

  double aDouble;
  string aString;
  po::options_description desc("Allowed options");
  desc.add_options()
    ("debug", "Display debug output")
    ("extras","Display intermediate steps")
    ("line", "Draw a line to the plane")
    ("record",po::value<string>(&aString)->default_value(""),"Record to directory [arg]")
    ("lat",po::value<double>(&aDouble)->default_value(-32.0),"Set tracker latitude (GPS Degrees)")
    ("lon",po::value<double>(&aDouble)->default_value(117.0),"Set tracker longitude (GPS Degrees)")
    ("alt",po::value<double>(&aDouble)->default_value(0.0),"Set tracker altitude (Meters)")
    ("scale",po::value<double>(&aDouble)->default_value(0.25),"Video scale")
    ("gps-port",po::value<string>(&aString)->default_value("/dev/tty0"),"Specify GPS serial port")
    ("arduino-port",po::value<string>(&aString)->default_value("/dev/tty.usbmodem1411"),"Specify arduino serial port")
    ("gps-baud",po::value<string>(&aString)->default_value("9600"),"Specify GPS serial baud rate")
    ("arduino-baud",po::value<string>(&aString)->default_value("57600"),"Specify arduino serial baud rate")
    ("video", po::value<string>(&aString)->default_value(""),"Simulate using video file [arg]")
    ("image", po::value<string>(&aString)->default_value(""), "Analyze a single image [arg]")
    ("blind","Do not use visual input")
    ("no-gps", "Do not use GPS")
    ("help", "Display help message");

  po::variables_map vm;
  po::store(po::command_line_parser(argc,argv).options(desc).run(),vm);
  po::notify(vm);

  if (vm.count("help")) {
    cout << desc << endl;
    return 0;
  }

  arguments.showExtras = vm.count("extras");
  arguments.drawLine = vm.count("line");
  arguments.blind = vm.count("blind");
  arguments.noGPS = vm.count("no-gps");
  arguments.debug = vm.count("debug");
  arguments.lat = vm["lat"].as<double>();
  arguments.lon = vm["lon"].as<double>();
  arguments.alt = vm["alt"].as<double>();
  arguments.scale = vm["scale"].as<double>();
  arguments.gpsBaud = baudRate(vm["gps-baud"].as<string>());
  arguments.arduinoBaud= baudRate(vm["arduino-baud"].as<string>());
  arguments.gpsPort= vm["gps-port"].as<string>();
  arguments.arduinoPort= vm["arduino-port"].as<string>();
  arguments.videoFilename = vm["video"].as<string>();
  arguments.imageFilename = vm["image"].as<string>();
  arguments.recordDirectory = vm["record"].as<string>();

  /* Initialize variables */
  Theron::Receiver                          imageReceiver;
  Vision*                                   vision;
  MultimodalActor*                          multimodalActor;
  FrameAnalyzerActor*                       frameAnalyzerActor;
  GeoreferencingActor*                      georeferencingActor;
  Theron::Catcher<PlaneVisionMessage>       imageCatcher;
  Theron::Address                           from;
  PlaneVisionMessage                        message;

  imageReceiver.RegisterHandler(&imageCatcher,&Theron::Catcher<PlaneVisionMessage>::Push);

  Log::log("Initializing tracker with the following settings: ");
  cout << "\tTracker Latitude: \t" << KMAG << arguments.lat << KNRM << endl;
  cout << "\tTracker Longitude: \t" << KMAG << arguments.lon << KNRM << endl;
  cout << "\tTracker Altitude: \t" << KMAG << arguments.alt << KNRM << endl;
  cout << "\tGPS Serial Device: \t" << KMAG << arguments.gpsPort << KNRM << endl;
  cout << "\tGPS Baud Rate: \t\t" << KMAG << arguments.gpsBaud << KNRM << endl;
  cout << "\tArduino Serial Device: \t" << KMAG << arguments.arduinoPort << KNRM << endl;
  cout << "\tArduino Baud Rate: \t" << KMAG << arguments.arduinoBaud << KNRM << endl << endl;

  if (arguments.videoFilename != ""){
    Log::log("\tSimulating flight using " + arguments.videoFilename);
  } else if (arguments.imageFilename != "") {
    Log::log("\tAnalyzing image: " + arguments.imageFilename);
  } else {
    Log::log("\tUsing live video");
  }

  if (arguments.showExtras) {
    Log::log("\tDisplaying intermediate steps");
    vision = new Vision(true);
  } else {
    vision = new Vision(false);
  }

  if (arguments.recordDirectory != "") {
    Log::log("\tRecording output to: " +  arguments.recordDirectory);
  }

  if (arguments.debug) {
    Log::debugMode = true;
    Log::debug("\tDebug output enabled");
  } else {
    Log::debugMode = false;
  }

  cout << endl;


  Log::log("Creating Theron Framework...");
  Theron::Framework framework;

  Log::log("Spawning Multimodal Actor...");
  multimodalActor = new MultimodalActor(framework,arguments.arduinoPort, B57600);


  if (!arguments.blind){
    Log::log("Spawning Frame Analyzer Actor...");
    frameAnalyzerActor = new FrameAnalyzerActor(
        framework,
        arguments.drawLine,
        vision,
        imageReceiver.GetAddress(),
        multimodalActor->GetAddress()
        );
    if (arguments.imageFilename == ""){
      Log::log("Spawning VideoReceiver Interface...");
      if (arguments.videoFilename == ""){
        new VideoReceiverInterface(
            framework,
            frameAnalyzerActor->GetAddress()
            );
      } else {
        new VideoReceiverInterface(
            framework,
            arguments.videoFilename,
            frameAnalyzerActor->GetAddress()
            );
      }
    } else {
      IplImage* image = cvLoadImage(arguments.imageFilename.c_str());
      framework.Send(
          ImageMessage(image),
          imageReceiver.GetAddress(),
          frameAnalyzerActor->GetAddress()
          );
    }
  } else {
    Log::log("Not using video input");
  }

  if (!arguments.noGPS){
    Log::log("Spawning Georeferencing Actor...");
    georeferencingActor = new GeoreferencingActor(framework,arguments.lat,arguments.lon,arguments.alt,multimodalActor->GetAddress());
    Log::log("Spawning GPSReceiver Interface..."); 
    new GPSReceiverInterface(framework, arguments.gpsPort, georeferencingActor->GetAddress());
  } else {
    Log::log("Not using GPS");
  }

  if (arguments.recordDirectory != "") {
    string command = string("mkdir ") + arguments.recordDirectory;
    system(command.c_str());
    Log::log("Recording to: " +  arguments.recordDirectory);
  }

  Log::success("Tracker Initialization Complete");

  auto threadFunct = [&](){
    unsigned int currentFrame = 0;
    while (true){
      currentFrame++;
      imageReceiver.Wait();
      imageReceiver.Consume(UINT_MAX);
      while (!imageCatcher.Empty()){
        imageCatcher.Pop(message,from);
        if (!imageCatcher.Empty()){
          cvReleaseImage(&message.result);
        }
      }
      for (unsigned int i = 0; i < message.extras.size(); i++){
        showImage(message.extras[i].name,message.extras[i].image, arguments.scale);
        cvReleaseImage(&message.extras[i].image);
      }
      showImage("Display window", message.result, arguments.scale);
      if (arguments.recordDirectory != "") {
        string frame = (boost::format("%06d") % currentFrame).str();
        string filename = arguments.recordDirectory + "/" + frame + ".jpg";
        Log::debug("Saving current frame to: " + filename);
        cvSaveImage(filename.c_str(), message.result);
      }
      cvReleaseImage(&message.result);
      cvWaitKey(1);
    } 
  };
  auto t = thread(threadFunct);

  return a.exec();
}

