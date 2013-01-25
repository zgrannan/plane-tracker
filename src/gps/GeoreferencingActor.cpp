#include "src/gps/GeoreferencingActor.h"
#include "src/gps/Georeference.h"
#include "src/util/Messages.h"
#include "src/util/Log.h"
#include <boost/lexical_cast.hpp>
#include <cmath>

void GeoreferencingActor::Handler(const GPSDataMessage& message, const Theron::Address){
  if (message.hasData){
    if ( _setPositionFromRadio ){
      setPosition(message.lat, message.lon, message.alt);
      _setPositionFromRadio = false;
      return;
    }
    const AbsolutePositionMessage positionMessage = calculateAbsolutePosition(message);
    if (!disabled)
      Send(positionMessage,multiModalActor);
  }
}
void GeoreferencingActor::setPosition(const double lat, const double lon, const double alt){
  Log::log("Setting tracker position to: " + boost::lexical_cast<string>(lat) + ", " +
           boost::lexical_cast<string>(lon) +" @ " + boost::lexical_cast<string>(alt) + "m");
  this->trackerLatitude = lat;
  this->trackerLongitude = lon;
  this->trackerAltitude = alt;
}

AbsolutePositionMessage GeoreferencingActor::calculateAbsolutePosition(const GPSDataMessage& message)
const{
  const auto result = GeoReference::calculatePanAndTilt(
    trackerLatitude, trackerLongitude, trackerAltitude,
    message.lat, message.lon, message.alt);

  return AbsolutePositionMessage(result.first,result.second);
}
