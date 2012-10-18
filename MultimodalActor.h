#ifndef FRAME_ANALYZER_ACTOR_H
#define FRAME_ANALYZER_ACTOR_H

#include <Theron/Theron.h>

class GPSDataMessage;

class MultimodalActor : public Theron::Actor {
public:
	explicit MultimodalActor(Theron::Framework &framework) : Theron::Actor(framework) {
		RegisterHandler(this, &Actor::Handler);
	}
private:
};

