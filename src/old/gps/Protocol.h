#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "src/util/Messages.h"

using namespace Messages;

namespace Protocol {
  Messages::GPSDataMessage parseSerialInputForGPS(const string input, string& extra);
  Messages::GPSDataMessage parseSerialInputLineForGPS(const string input);
  double parseLatitudeOrLongitude(const string input);
}

#endif
