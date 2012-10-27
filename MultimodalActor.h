#ifndef MULTIMODAL_ACTOR_H
#define MULTIMODAL_ACTOR_H

#include <Theron/Theron.h>
#include <iostream>
#include <boost/asio.hpp>
#include "Messages.h"

using namespace std;
using namespace Messages;

class MultimodalActor : public Theron::Actor {
  public:
    explicit MultimodalActor(Theron::Framework &framework) : Theron::Actor(framework) {
      cerr << "Opening serial port...\n";
      fd = open("/dev/ttys1", O_RDWR | O_NOCTTY | O_NDELAY);
      cerr << "Registering handlers...\n";
      RegisterHandler(this, &MultimodalActor::GPSHandler);
      RegisterHandler(this, &MultimodalActor::VisionHandler);
    }
  private:
    void GPSHandler(const AbsolutePositionMessage &message, const Theron::Address from);
    void VisionHandler(const RelativePositionMessage &message, const Theron::Address from);
    void instructGimbal(const PositionMessage &message);
    int fd;
};

#endif
