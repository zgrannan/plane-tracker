#include <boost/lexical_cast.hpp>
#include "MultimodalActor.h"
#include "Protocol.h"
#include "Messages.h"
#include "Log.h"

using namespace boost;

void MultimodalActor::GPSHandler(const AbsolutePositionMessage &message, const Theron::Address from){
  Log::debug("AbsolutePositionMessage received");
  if (!message.positionLost){
    gpsLost = false;
    useRSSI = false;
    Log::debug("Pan: " + lexical_cast<string>(message.pan) + " Tilt: " + lexical_cast<string>(message.tilt));
    if (videoLost)
      instructGimbal(message);
  } else {
    Log::debug("GPS Lost");
    gpsLost = true;
    if (videoLost == true && useRSSI == false){
      Log::debug("Switching to RSSI");
      useRSSI = true;
      instructGimbal(UseRSSIMessage());
    }
  }
}

void MultimodalActor::VisionHandler(const RelativePositionMessage &message, const Theron::Address from){
  Log::debug("RelativePositionMessage received");
  if (!message.positionLost){
    videoLost = false;
    useRSSI = false;
    Log::debug("Pan: " + lexical_cast<string>(message.pan) + " Tilt: " + lexical_cast<string>(message.tilt));
    instructGimbal(message);
  } else {
    Log::debug("Video Lost");
    videoLost = true;
    if (videoLost == true && useRSSI == false){
      Log::debug("Switching to RSSI");
      useRSSI = true;
      instructGimbal(UseRSSIMessage());
    }
  }
}

void MultimodalActor::instructGimbal(const PositionMessage &message){
  const vector<char> bytes = message.toBytes();
  const char* bytePtr = &bytes[0];
  Log::debug(string("Writing message: ") + bytePtr);
  write(fd, bytePtr, bytes.size());
}
