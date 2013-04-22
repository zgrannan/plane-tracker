#ifndef VIDEORECIEVERINTERFACE_H
#define VIDEORECIEVERINTERFACE_H

#include <Theron/Theron.h>
#include <cv.h>

using namespace cv;

class VideoReceiverInterface {
public:
	VideoReceiverInterface(Theron::Framework &framework, string videoFilename, Theron::Address frameAnalyzerActor);
    VideoReceiverInterface(Theron::Framework &framework,
                           Theron::Address frameAnalyzerActor);
    void sendImage(IplImage* image);
private:
    void cameraFunction();
    void videoFunction(string videoFilename);
    Theron::Framework& framework;
    Theron::Address frameAnalyzerActor;
    Theron::Receiver receiver;
};

#endif
