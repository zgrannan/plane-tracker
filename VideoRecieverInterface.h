#ifndef VIDEORECIEVERINTERFACE_H
#define VIDEORECIEVERINTERFACE_H
#include <Theron/Theron.h>

class IplImage;

class VideoRecieverInterface {
  VideoRecieverInterface(Theron::Framework framework, Theron::Address frameAnalyzerActor);
private:
	void sendImage(IplImage* image);
    void workerFunction();
    Theron::Framework framework;
    Theron::Address frameAnalyzerActor;
    Theron::Receiver reciever;
};

#endif
