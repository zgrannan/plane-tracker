#ifndef FRAME_ANALYZER_ACTOR_H
#define FRAME_ANALYZER_ACTOR_H

#include <Theron/Theron.h>

class GPSDataMessage;

class GeoreferencingActor : public Theron::Actor {
public:
	explicit Actor(Theron::Framework &framework, double lat, double lon, double alt, Theron::Address multiModalActor) : Theron::Actor(framework) {
        trackerLatitude = lat;
        trackerLongitude = lon;
        trackerAltitude = alt;
		RegisterHandler(this, &Actor::Handler);
	}
private:
	void Handler(const GPSDataMessage* message, const Theron::Address sender);
	AbsolutePositionMessage calculateAbsolutePosition(GPSDataMessage * gpsData);
    const double trackerLatitude,trackerLongitude,trackerAltitude;
    Theron::Address multiModalActor;
};

#endif
