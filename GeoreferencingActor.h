#ifndef GEOREFERENCING_ACTOR_H
#define GEOREFERENCING_ACTOR_H

#include <Theron/Theron.h>
#include "Messages.h"

using namespace Messages;

class GeoreferencingActor : public Theron::Actor {
public:
	GeoreferencingActor(Theron::Framework &framework,
                        double lat,
                        double lon,
                        double alt,
                        Theron::Address multiModalActor) : 
      Theron::Actor(framework),
      trackerLatitude(lat),
      trackerLongitude(lon),
      trackerAltitude(alt),
      multiModalActor(multiModalActor){
		RegisterHandler(this, &GeoreferencingActor::Handler);
	}
    bool disable(){disabled = true; }
    bool enable(){disabled = false; }
    void setPosition(double lat, double lon, double alt){
      this->trackerLatitude = lat;
      this->trackerLongitude = lon;
      this->trackerAltitude = alt;
    }
private:
    bool disabled = false;
	void Handler(const GPSDataMessage& message, const Theron::Address from);
	AbsolutePositionMessage calculateAbsolutePosition(const GPSDataMessage& gpsData);
    double trackerLatitude,trackerLongitude,trackerAltitude;
    Theron::Address multiModalActor;
};

#endif
