#include "VideoReceiverInterface.h"
#include "Messages.h"
#include <boost/thread.hpp>
#include <cv.h>
#include <highgui.h>
#include <iostream>


using namespace cv;
using namespace std;
using namespace Messages;

void VideoReceiverInterface::sendImage(IplImage* image){
  const ImageMessage message(image);
  cerr <<"Sending message";
  framework.Send(message,receiver.GetAddress(),frameAnalyzerActor);
  cerr <<"Sent";
}

void VideoReceiverInterface::workerFunction(){
  boost::posix_time::seconds sleepTime(3);
  IplImage* image = cvLoadImage("plane.jpg");
  while(true){
    boost::this_thread::sleep(sleepTime);
    sendImage(image);
  }
}

VideoReceiverInterface::VideoReceiverInterface(Theron::Framework &framework, Theron::Address frameAnalyzerActor): framework(framework), frameAnalyzerActor(frameAnalyzerActor) {
  boost::thread workerThread(&VideoReceiverInterface::workerFunction, this);
}
