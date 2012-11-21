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
  vector<double> resultVector = GeoReference::calculateBearingAndDistance(trackerLatitude,trackerLongitude,message.lat,message.lon);
  double bearing = resultVector[0];
  double distance = resultVector[1];
  double tilt = tan(message.alt/distance)*180/3.14159;
  return AbsolutePositionMessage(bearing,tilt);
}
