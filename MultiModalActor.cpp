#include "MultimodalActor.h"
#include "Protocol.h"
#include "Messages.h"
#include "Log.h"

void MultimodalActor::GPSHandler(const AbsolutePositionMessage &message, const Theron::Address from){
  Log::debug("AbsolutePositionMessage received");
  instructGimbal(message);
}
void MultimodalActor::VisionHandler(const RelativePositionMessage &message, const Theron::Address from){
  Log::debug("RelativePositionMessage received");
  instructGimbal(message);
}

void MultimodalActor::instructGimbal(const PositionMessage &message){
  const vector<char> bytes = message.toBytes();
  const char* bytePtr = &bytes[0];
  write(fd, bytePtr, bytes.size());
}
