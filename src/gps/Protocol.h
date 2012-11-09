#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "src/util/Messages.h"

using namespace Messages;

namespace Protocol {
  Messages::GPSDataMessage parseSerialInputForGPS(string input, string& extra);
  Messages::GPSDataMessage parseSerialInputLineForGPS(string input);
  double parseLatitudeOrLongitude(string input);
}

#endif
