#include <QtGui/QApplication>
#include <Theron/Theron.h>
#include <boost/format.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <thread>
#include "src/gps/GPSReceiverInterface.h"
#include "src/gps/GeoreferencingActor.h"
#include "src/main/MultiModalActor.h"
#include "src/ui/ui.h"
#include "src/util/Log.h"
#include "src/vision/FrameAnalyzerActor.h"
#include "src/vision/VideoReceiverInterface.h"
#include "src/vision/Vision.h"

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
  struct arguments{
    bool blind;
    bool debug;
    bool drawLine;
    bool noGPS;
    bool noGUI;
    bool showExtras;
    double alt; 
    double lat; 
    double lon; 
    double scale; 
    string arduinoBaud;
    string gpsBaud;
    string arduinoPort; 
    string gpsPort; 
    string imageFilename;
    string recordDirectory;
    string videoFilename;
  } arguments;

  po::options_description desc("Allowed options");
  desc.add_options()
    ("alt",po::value<double>(&arguments.alt)->default_value(0.0),"Set tracker altitude (Meters)")
    ("arduino-baud",po::value<string>(&arguments.arduinoBaud)->default_value("57600"),"Specify arduino serial baud rate")
    ("arduino-port",po::value<string>(&arguments.arduinoPort)->default_value("/dev/tty.usbmodem1411"),"Specify arduino serial port")
    ("blind","Do not use visual input")
    ("debug", "Display debug output")
    ("extras","Display intermediate steps")
    ("gps-baud",po::value<string>(&arguments.gpsBaud)->default_value("9600"),"Specify GPS serial baud rate")
    ("gps-port",po::value<string>(&arguments.gpsPort)->default_value("/dev/tty0"),"Specify GPS serial port")
    ("help", "Display help message")
    ("image", po::value<string>(&arguments.imageFilename)->default_value(""), "Analyze a single image [arg]")
    ("lat",po::value<double>(&arguments.lat)->default_value(-32.0),"Set tracker latitude (GPS Degrees)")
    ("line", "Draw a line to the plane")
    ("lon",po::value<double>(&arguments.lon)->default_value(117.0),"Set tracker longitude (GPS Degrees)")
    ("no-gps", "Do not use GPS")
    ("no-gui", "Do not use GUI")
    ("record",po::value<string>(&arguments.recordDirectory)->default_value(""),"Record to directory [arg]")
    ("scale",po::value<double>(&arguments.scale)->default_value(0.25),"Video scale")
    ("video", po::value<string>(&arguments.videoFilename)->default_value(""),"Simulate using video file [arg]");
  po::variables_map vm;
  auto parsedOptions = po::command_line_parser(argc,argv).options(desc).allow_unregistered().run();
  auto invalidOptions = collect_unrecognized(parsedOptions.options,po::include_positional);
  if (invalidOptions.size() > 0){
    Log::error("Invalid Option: " + invalidOptions[0]);
    cout << endl << desc << endl;
    return 0;
  }
  po::store(parsedOptions,vm);
  po::notify(vm);
  cerr << "done\n";

  if (vm.count("help")) {
    cout << desc << endl;
    return 0;
  }

  arguments.alt = vm["alt"].as<double>();
  arguments.arduinoBaud= vm["arduino-baud"].as<string>();
  arguments.arduinoPort= vm["arduino-port"].as<string>();
  arguments.blind = vm.count("blind");
  arguments.debug = vm.count("debug");
  arguments.drawLine = vm.count("line");
  arguments.gpsBaud = vm["gps-baud"].as<string>();
  arguments.gpsPort= vm["gps-port"].as<string>();
  arguments.imageFilename = vm["image"].as<string>();
  arguments.lat = vm["lat"].as<double>();
  arguments.lon = vm["lon"].as<double>();
  arguments.noGPS = vm.count("no-gps");
  arguments.noGUI = vm.count("no-gui");
  arguments.recordDirectory = vm["record"].as<string>();
  arguments.scale = vm["scale"].as<double>();
  arguments.showExtras = vm.count("extras");
  arguments.videoFilename = vm["video"].as<string>();

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

  Log::log("Spawning Frame Analyzer Actor...");
  frameAnalyzerActor = new FrameAnalyzerActor(
      framework,
      arguments.drawLine,
      vision,
      imageReceiver.GetAddress(),
      multimodalActor->GetAddress()
      );

  if (!arguments.blind){
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

  Log::log("Spawning Georeferencing Actor...");
  georeferencingActor = new GeoreferencingActor(framework,arguments.lat,arguments.lon,arguments.alt,multimodalActor->GetAddress());
  if (!arguments.noGPS){
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

  if (!arguments.noGUI){
    QApplication a(argc,argv);
    UI ui(nullptr,frameAnalyzerActor,georeferencingActor,multimodalActor);
    ui.show();
    return a.exec();
  } else {
    Log::log("Press any key to quit");
    getchar();
    return 0;
  }
}

