#ifndef MULTIMODAL_ACTOR_H
#define MULTIMODAL_ACTOR_H

#include <Theron/Theron.h>
#include <iostream>
#include <boost/asio.hpp>
#include "src/util/Messages.h"
#include "src/util/Log.h"

using namespace std;
using namespace Messages;
using namespace boost::posix_time;

class MultimodalActor : public Theron::Actor {
  public:
    explicit MultimodalActor(Theron::Framework &framework,
                             const string serialPort, 
                             const int baudrate); 
    void instructGimbal(const PositionMessage &message);
    void setAmplification(const double amplification);
  private:
    void GPSHandler(const AbsolutePositionMessage &message, const Theron::Address from);
    void VisionHandler(const RelativePositionMessage &message, const Theron::Address from);
    AbsolutePositionMessage lastGPSMessage;
    RelativePositionMessage lastVisionMessage;
    boost::posix_time::ptime lastInstructionTime;
    int fd;
    bool useRSSI;
    bool videoLost;
    bool gpsLost;
    double amplification = 1.0;
};

#endif
