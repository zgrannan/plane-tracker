#ifndef FRAME_ANALYZER_ACTOR_H
#define FRAME_ANALYZER_ACTOR_H

#include <Theron/Theron.h>
#include "Messages.h"

class GeoreferencingActor : public Theron::Actor {
public:
	GeoreferencingActor(Theron::Framework &framework, double lat, double lon, double alt, Theron::Address multiModalActor) : Theron::Actor(framework) {
        trackerLatitude = lat;
        trackerLongitude = lon;
        trackerAltitude = alt;
		RegisterHandler(this, &GeoreferencingActor::Handler);
	}
private:
	void Handler(const GPSDataMessage& message, const Theron::Address from);
	AbsolutePositionMessage calculateAbsolutePosition(GPSDataMessage * gpsData);
    double trackerLatitude,trackerLongitude,trackerAltitude;
    Theron::Address multiModalActor;
};

#endif
