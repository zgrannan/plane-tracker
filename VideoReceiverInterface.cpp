#include "VideoRecieverInterface.h"

VideoRecieverInterface::VideoRecieverInterface(Theron::Framework framework, Theron::Address frameAnalyzerActor){
  this->framework = framework;
  this->frameAnalyzerActor = frameAnalyzerActor;
  boost::thread workerThread(workerFunction);
}

void VideoRecieverInferface::sendImage(IplImage* image){
  framework.send(image,reciever,frameAnalyzerActor);
}

void VideoRecieverInterface::workerThread(){
  boost::posix_time::seconds sleepTime(3);
  IplImage* image = cvLoadImage(argv[1]);
  while(true){
    boost::this_thread::sleep(sleepTime);
    sendImage(image);
  }
}
