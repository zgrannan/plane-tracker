#include "VideoReceiverInterface.h"
#include <boost/thread.hpp>
#include <cv.h>
#include <highgui.h>

using namespace cv;

void VideoReceiverInterface::sendImage(IplImage* image){
  framework.Send(*image,receiver.GetAddress(),frameAnalyzerActor);
}

void VideoReceiverInterface::workerFunction(){
  boost::posix_time::seconds sleepTime(3);
  IplImage* image = cvLoadImage("testImage.jpg");
  while(true){
    boost::this_thread::sleep(sleepTime);
    sendImage(image);
  }
}

VideoReceiverInterface::VideoReceiverInterface(Theron::Framework &framework, Theron::Address frameAnalyzerActor): framework(framework), frameAnalyzerActor(frameAnalyzerActor) {
  boost::thread workerThread(&VideoReceiverInterface::workerFunction, this);
}
