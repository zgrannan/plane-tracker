#ifndef MULTIMODAL_ACTOR_H
#define MULTIMODAL_ACTOR_H

#include <Theron/Theron.h>
#include <boost/asio.hpp>
#include "Messages.h"

using namespace Messages;

class MultimodalActor : public Theron::Actor {
public:
	explicit MultimodalActor(Theron::Framework &framework) : Theron::Actor(framework) {
        fd = fopen("/dev/ttys0","w");
		RegisterHandler(this, &MultimodalActor::GPSHandler);
        RegisterHandler(this, &MultimodalActor::VisionHandler);
	}
private:
  void GPSHandler(const AbsolutePositionMessage &message, const Theron::Address from);
  void VisionHandler(const RelativePositionMessage &message, const Theron::Address from);
  void instructGimbal(const PositionMessage &message);
  FILE* fd;
};

#endif
