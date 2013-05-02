#include <cv.h>
#include <boost/thread.hpp>
#include <highgui.h>
#include <iostream>
#include "src/vision/VideoReceiverInterface.h"
#include "src/util/Messages.h"
#include "src/util/Log.h"

using namespace cv;
using namespace std;
using namespace Messages;

void VideoReceiverInterface::sendImage(IplImage* image){
  const ImageMessage message(image);
  DEBUG("Sending imageMessage to FrameAnalyzerActor");
  framework.Send(message,receiver.GetAddress(),frameAnalyzerActor);
}

void VideoReceiverInterface::cameraFunction(){
  VideoCapture capture(0);
  Mat temp;
  IplImage* image;
  while(true){
    capture >> temp;
    IplImage tempIpl = temp;
    image = new IplImage(tempIpl);
    IplImage* halfImage = cvCreateImage(cvSize(image->width/2, image->height/2), image->depth, image->nChannels);
    cvResize(image, halfImage);
    sendImage(halfImage);
  }
}

void VideoReceiverInterface::videoFunction(string videoFilename){
  int startFrame = 0;
  int framerate = 10;
  VideoCapture capture(videoFilename);
  auto numFrames = capture.get(CV_CAP_PROP_FRAME_COUNT);
  Mat temp;
  IplImage* image;
  for(int i = 0; i < numFrames; i++) {
    capture >> temp;
    IplImage tempIpl = temp;
    if ( i > startFrame) {
      image = new IplImage(tempIpl); 
      sendImage(cvCloneImage(image));
      usleep(1000000 / framerate); }
  }
}

VideoReceiverInterface::VideoReceiverInterface(Theron::Framework &framework, string videoFilename, Theron::Address frameAnalyzerActor): framework(framework), frameAnalyzerActor(frameAnalyzerActor) {
  boost::thread workerThread(&VideoReceiverInterface::videoFunction, this, videoFilename);
}

VideoReceiverInterface::VideoReceiverInterface(Theron::Framework &framework, Theron::Address frameAnalyzerActor): framework(framework), frameAnalyzerActor(frameAnalyzerActor) {
  boost::thread workerThread(&VideoReceiverInterface::cameraFunction, this);
}
