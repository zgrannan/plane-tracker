#ifndef GEOREFERENCING_ACTOR_H
#define GEOREFERENCING_ACTOR_H

#include <Theron/Theron.h>
#include "src/util/Messages.h"

using namespace Messages;

class GeoreferencingActor : public Theron::Actor {
public:
	GeoreferencingActor(Theron::Framework &framework,
                        const double lat,
                        const double lon,
                        const double alt,
                        Theron::Address multiModalActor) : 
      Theron::Actor(framework),
      trackerLatitude(lat),
      trackerLongitude(lon),
      trackerAltitude(alt),
      multiModalActor(multiModalActor){
		RegisterHandler(this, &GeoreferencingActor::Handler);
	}
    void disable(){disabled = true; }
    void enable(){disabled = false; }
    void setPosition(const double lat, const double lon, const double alt);
    void setPositionFromRadio(){
      _setPositionFromRadio = true;
    }
private:
    bool _setPositionFromRadio = false;
    bool disabled = true;
	void Handler(const GPSDataMessage& message, const Theron::Address from);
	AbsolutePositionMessage calculateAbsolutePosition(const GPSDataMessage& gpsData) const;
    double trackerLatitude,trackerLongitude,trackerAltitude;
    const Theron::Address multiModalActor;
};

#endif
