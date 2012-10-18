#ifndef FRAME_ANALYZER_ACTOR_H
#define FRAME_ANALYZER_ACTOR_H

#include <Theron/Theron.h>

class GPSDataMessage;

class GeoreferencingActor : public Theron::Actor {
public:
	explicit Actor(Theron::Framework &framework) : Theron::Actor(framework) {
		RegisterHandler(this, &Actor::Handler);
	}
private:
	void Handler(const ImageMessage* message, const Theron::Address sender);
	vector<double> calculateAbsolutePosition(double lat, double lon, double alt);
};

