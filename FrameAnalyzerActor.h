#ifndef FRAME_ANALYZER_ACTOR_H
#define FRAME_ANALYZER_ACTOR_H

#include <Theron/Theron.h>
#include "Messages.h"

class FrameAnalyzerActor : public Theron::Actor {
public:
	FrameAnalyzerActor(Theron::Framework &framework, Theron::Address multimodalActor) : Theron::Actor(framework) {
		RegisterHandler(this, &FrameAnalyzerActor::Handler);
	}
private:
    RelativePositionMessage calculateRelativePosition(ImageMessage* message);
	void Handler(const ImageMessage& message, const Theron::Address from){}
    Theron::Address multimodalActor;
};

#endif
