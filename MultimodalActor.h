#ifndef MULTIMODAL_ACTOR_H
#define MULTIMODAL_ACTOR_H

#include <Theron/Theron.h>
#include <boost/asio.hpp>
#include "Messages.h"

using namespace Messages;

class MultimodalActor : public Theron::Actor {
public:
	explicit MultimodalActor(Theron::Framework &framework) : Theron::Actor(framework), io(),port(io,"/dev/ttys0") {
		RegisterHandler(this, &MultimodalActor::GPSHandler);
        RegisterHandler(this, &MultimodalActor::VisionHandler);
	}
private:
  void GPSHandler(const AbsolutePositionMessage &message, const Theron::Address sender);
  void VisionHandler(const RelativePositionMessage &message, const Theron::Address sender);
  void instructGimbal(const PositionMessage &message);
  boost::asio::io_service io;
  boost::asio::serial_port port;
};

#endif
