#ifndef FRAME_ANALYZER_ACTOR_H
#define FRAME_ANALYZER_ACTOR_H

#include <Theron/Theron.h>
#include "Messages.h"
#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace Messages;

class FrameAnalyzerActor : public Theron::Actor {
public:
	FrameAnalyzerActor(Theron::Framework &framework,Theron::Address imageViewer, Theron::Address multimodalActor) : Theron::Actor(framework), imageViewer(imageViewer), multimodalActor(multimodalActor){
		RegisterHandler(this, &FrameAnalyzerActor::Handler);
	}
private:
    RelativePositionMessage calculateRelativePosition(const ImageMessage& message);
	void Handler(const ImageMessage& message, const Theron::Address from);
    Theron::Address multimodalActor;
    Theron::Address imageViewer;
};

#endif
