#ifndef FRAME_ANALYZER_ACTOR_H
#define FRAME_ANALYZER_ACTOR_H

#include <Theron/Theron.h>

class GPSDataMessage;

class MultimodalActor : public Theron::Actor {
public:
	explicit MultimodalActor(Theron::Framework &framework) : Theron::Actor(framework) {
        this->framework = framework;
        port.open("/dev/ttys1");
		RegisterHandler(this, &Actor::GPSHandler);
        RegisterHandler(this, &Actor::VisionHandler);
	}
private:
  void GPSHandler(const AbsolutePositionMessage &message, const Theron::Address sender);
  void VisionHandler(const RelativePositionMessage &message, const Theron::Address sender);
  void instructGimbal(const PositionMessage &message);
  boost::serial_port port;
};

