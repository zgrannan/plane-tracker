#include <Theron/Theron.h>
#include <boost/lexical_cast.hpp>
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
#include "src/util/Log.h"
#include "src/vision/FrameAnalyzerActor.h"
#include "src/vision/VideoReceiverInterface.h"
#include "src/vision/Vision.h"
#include "src/ui/ui.h"
#include "src/ui/imageview.h"
#include <QtGui/QApplication>

using namespace cv;
using namespace std;
using namespace Messages;

namespace po = boost::program_options;

/**
 * Displays the image on the screen for debugging purposes
 */
void showImage(ImageView* imageView, IplImage* image, int width, int height){
  CvSize newSize = cvSize(width,height);
  IplImage* newImage = cvCreateImage(newSize,image->depth,image->nChannels);
  cvResize(image,newImage); 
  imageView->sendImage(newImage);
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
    bool debug;
    bool useCompositeInput;
    bool drawLine;
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
    ("composite","Use composite input instead of HDMI")
    ("debug", "Display debug output")
    ("extras","Display intermediate steps")
    ("gps-baud",po::value<string>(&arguments.gpsBaud)->default_value("9600"),"Specify GPS serial baud rate")
    ("gps-port",po::value<string>(&arguments.gpsPort)->default_value("/dev/tty0"),"Specify GPS serial port")
    ("help", "Display help message")
    ("image", po::value<string>(&arguments.imageFilename)->default_value(""), "Analyze a single image [arg]")
    ("lat",po::value<double>(&arguments.lat)->default_value(-32.0),"Set tracker latitude (GPS Degrees)")
    ("line", "Draw a line to the plane")
    ("lon",po::value<double>(&arguments.lon)->default_value(117.0),"Set tracker longitude (GPS Degrees)")
    ("record",po::value<string>(&arguments.recordDirectory)->default_value(""),"Record to directory [arg]")
    ("scale",po::value<double>(&arguments.scale)->default_value(0.0),"Video scale")
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
  arguments.useCompositeInput = vm.count("composite");
  arguments.debug = vm.count("debug");
  arguments.drawLine = vm.count("line");
  arguments.gpsBaud = vm["gps-baud"].as<string>();
  arguments.gpsPort= vm["gps-port"].as<string>();
  arguments.imageFilename = vm["image"].as<string>();
  arguments.lat = vm["lat"].as<double>();
  arguments.lon = vm["lon"].as<double>();
  arguments.recordDirectory = vm["record"].as<string>();
  arguments.scale = vm["scale"].as<double>();
  arguments.showExtras = vm.count("extras");
  arguments.videoFilename = vm["video"].as<string>();

  if (arguments.scale <= 0.0){
    if (arguments.useCompositeInput){
      arguments.scale = 1.0;
    } else {
      arguments.scale = 0.5;
    }
  }

  /* Initialize variables */
  Theron::Receiver                          imageReceiver;
  Vision*                                   vision;
  MultimodalActor*                          multimodalActor;
  FrameAnalyzerActor*                       frameAnalyzerActor;
  GeoreferencingActor*                      georeferencingActor;
  Theron::Catcher<PlaneVisionMessage>       imageCatcher;
  Theron::Address                           from;
  PlaneVisionMessage                        message;

  int displayWindowWidth, displayWindowHeight;


  if ( arguments.useCompositeInput ){
    displayWindowWidth = 720 * arguments.scale;
    displayWindowHeight = 486 * arguments.scale;
  } else {
    displayWindowWidth = 1920 * arguments.scale;
    displayWindowHeight = 1080 * arguments.scale;
  }

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
  multimodalActor = new MultimodalActor(framework,arguments.arduinoPort, baudRate(arguments.arduinoBaud));

  Log::log("Spawning Frame Analyzer Actor...");
  frameAnalyzerActor = new FrameAnalyzerActor(
      framework,
      arguments.drawLine,
      vision,
      imageReceiver.GetAddress(),
      multimodalActor->GetAddress()
      );


  Log::log("Spawning Georeferencing Actor...");
  georeferencingActor = new GeoreferencingActor(framework,arguments.lat,arguments.lon,arguments.alt,multimodalActor->GetAddress());

  if (arguments.recordDirectory != "") {
    string command = string("mkdir ") + arguments.recordDirectory;
    system(command.c_str());
    Log::log("Recording to: " +  arguments.recordDirectory);
  }


  Log::log("Initializaing QApplication...");
  QApplication a(argc,argv);
  UI ui(nullptr,frameAnalyzerActor,georeferencingActor,multimodalActor);
  vector<ImageView*> extraViews;

  if (arguments.showExtras){
    for (int i = 0; i < 3; i ++){
      extraViews.push_back(new ImageView(&ui,
                                         displayWindowWidth,
                                         displayWindowHeight,
                                         arguments.scale,
                                         frameAnalyzerActor->GetAddress(),
                                         framework));
      extraViews[i]->show();
    }
  }

  ImageView* imageView = new ImageView(&ui,
                                       displayWindowWidth,
                                       displayWindowHeight,
                                       arguments.scale,
                                       frameAnalyzerActor->GetAddress(),
                                       framework);
  imageView->show();

  ui.show();

  auto threadFunct = [&](){
    unsigned int currentFrame = 0;
    while (true){
      currentFrame++;
      imageReceiver.Wait();
      while (!imageCatcher.Empty()){
        imageCatcher.Pop(message,from);
        if (!imageCatcher.Empty()){
          Log::debug("Skipped a frame in video playback");
          cvReleaseImage(&message.result);
          for (auto extra : message.extras ){
            cvReleaseImage(&extra.image);
          }
        }
      }

      for (unsigned int i = 0; i < message.extras.size(); i++){
        auto extra = message.extras[i];
        showImage(extraViews[i],extra.image, displayWindowWidth, displayWindowHeight);
        cvReleaseImage(&extra.image);
      }

      showImage(imageView, message.result, displayWindowWidth, displayWindowHeight);
      if (arguments.recordDirectory != "") {
        string frame = (boost::format("%06d") % currentFrame).str();
        string filename = arguments.recordDirectory + "/" + frame + ".bmp";
        Log::debug("Saving current frame to: " + filename);
        cvSaveImage(filename.c_str(), message.result);
      }
      cvReleaseImage(&message.result);
    } 
  };

  auto t = thread(threadFunct);

  Log::log("Spawning GPSReceiver Interface..."); 
  new GPSReceiverInterface(framework, arguments.gpsPort, baudRate(arguments.gpsBaud), georeferencingActor->GetAddress());
  if (arguments.imageFilename == ""){
    Log::log("Spawning VideoReceiver Interface...");
    if (arguments.videoFilename == ""){
      new VideoReceiverInterface(
          framework,
          frameAnalyzerActor->GetAddress(),
          arguments.useCompositeInput
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

  Log::success("Tracker Initialization Complete");
  return a.exec();
}

