#ifndef MULTIMODAL_ACTOR_H
#define MULTIMODAL_ACTOR_H

#include <Theron/Theron.h>
#include <iostream>
#include <boost/asio.hpp>
#include "Messages.h"
#include "Log.h"

using namespace std;
using namespace Messages;
using namespace boost::posix_time;

class MultimodalActor : public Theron::Actor {
  public:
    explicit MultimodalActor(Theron::Framework &framework, string serialPort) : 
      useRSSI(true),
      videoLost(true),
      gpsLost(true),
      Theron::Actor(framework) {
        Log::debug("Multimodal actor opening serial port: " +serialPort);
        fd = open(serialPort.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
        RegisterHandler(this, &MultimodalActor::GPSHandler);
        RegisterHandler(this, &MultimodalActor::VisionHandler);
    }
  private:
    void GPSHandler(const AbsolutePositionMessage &message, const Theron::Address from);
    void VisionHandler(const RelativePositionMessage &message, const Theron::Address from);
    void instructGimbal(const PositionMessage &message);
    int fd;
    bool useRSSI;
    bool videoLost;
    bool gpsLost;
};

#endif
