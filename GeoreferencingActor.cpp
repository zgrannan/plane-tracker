#include "GeoreferencingActor.h"
#include "Georeference.h"
#include "Messages.h"

void GeoreferencingActor::Handler(const GPSDataMessage& message, const Theron::Address from){
  AbsolutePositionMessage positionMessage = calculateAbsolutePosition(message);
  Send(positionMessage,multiModalActor);
}

AbsolutePositionMessage GeoreferencingActor::calculateAbsolutePosition(const GPSDataMessage& message){
  double roll,pitch;
  GeoReference::reverseGeoreference(trackerLatitude,trackerLongitude,trackerAltitude,0,0,0,message.lat,message.lon,message.lon,roll,pitch);
  return AbsolutePositionMessage(roll,pitch);
}
