#ifndef VIDEORECIEVERINTERFACE_H
#define VIDEORECIEVERINTERFACE_H

#include <Theron/Theron.h>
#include <cv.h>

using namespace cv;

class VideoReceiverInterface {
public:
  VideoReceiverInterface(Theron::Framework &framework, Theron::Address frameAnalyzerActor);
private:
	void sendImage(IplImage* image);
    void workerFunction();
    Theron::Framework& framework;
    Theron::Address frameAnalyzerActor;
    Theron::Receiver receiver;
};

#endif
