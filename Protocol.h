#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "Messages.h"

using namespace Messages;

namespace Protocol {
  Messages::GPSDataMessage parseSerialInputForGPS(string input);
  double parseLatitudeOrLongitude(string input);
}

#endif
