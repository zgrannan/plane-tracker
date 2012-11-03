#include "MultimodalActor.h"
#include "Protocol.h"
#include "Messages.h"

void MultimodalActor::GPSHandler(const AbsolutePositionMessage &message, const Theron::Address from){
  instructGimbal(message);
}
void MultimodalActor::VisionHandler(const RelativePositionMessage &message, const Theron::Address from){
  cerr << "Message received\n";
  instructGimbal(message);
}

void MultimodalActor::instructGimbal(const PositionMessage &message){
  const vector<char> bytes = message.toBytes();
  const char* bytePtr = &bytes[0];
  cout <<"Received position message: "<<bytePtr;
  write(fd, bytePtr, bytes.size());
}
