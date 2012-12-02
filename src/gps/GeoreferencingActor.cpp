#include "src/gps/GeoreferencingActor.h"
#include "src/gps/Georeference.h"
#include "src/util/Messages.h"
#include <cmath>

void GeoreferencingActor::Handler(const GPSDataMessage& message, const Theron::Address){
  if (message.hasData){
    const AbsolutePositionMessage positionMessage = calculateAbsolutePosition(message);
    if (!disabled)
      Send(positionMessage,multiModalActor);
  }
}

AbsolutePositionMessage GeoreferencingActor::calculateAbsolutePosition(const GPSDataMessage& message){
  auto result = GeoReference::calculatePanAndTilt(
    trackerLatitude, trackerLongitude, trackerAltitude,
    message.lat, message.lon, message.alt);

  return AbsolutePositionMessage(result.first,result.second);
}
