#include "MultimodalActor.h"


void MultimodalActor::GPSHandler(const AbsolutePositionMessage &message, const Theron::Address sender){
  instructGimbal(message);
}
void MultimodalActor::VisionHandler(const RelativePositionMessage &message, const Theron::Address sender){
  instructGimbal(message);
}
void MultimodalActor::instructGimbal(const PositionMessage &message){
  vector<byte> bytes = Protocol::getSerialBytesForArduino(message);
  port.write(bytes);
}
