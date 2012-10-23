#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "Messages.h"

namespace Protocol {
  Messages::GPSDataMessage parseSerialInputForGPS(string input);
  double parseLatitudeOrLongitude(string input);
  vector<char> getSerialBytesForArduino(const AbsolutePositionMessage &message);
  vector<char> getSerialBytesForArduino(const RelativePositionMessage &message);
}

#endif
