#ifndef FRAME_ANALYZER_ACTOR_H
#define FRAME_ANALYZER_ACTOR_H

#include <Theron/Theron.h>
#include <boost/asio.hpp>
#include "Messages.h"

class MultimodalActor : public Theron::Actor {
public:
	explicit MultimodalActor(Theron::Framework &framework) : Theron::Actor(framework) {
        //port.open("/dev/ttys1");
		RegisterHandler(this, &MultimodalActor::GPSHandler);
        RegisterHandler(this, &MultimodalActor::VisionHandler);
	}
private:
  void GPSHandler(const AbsolutePositionMessage &message, const Theron::Address sender);
  void VisionHandler(const RelativePositionMessage &message, const Theron::Address sender);
  void instructGimbal(const PositionMessage &message);
};

#endif
