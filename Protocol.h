#ifndef PROTOCOL_H
#define PROTOCOL_H

class std::string;
class GPSDataMessage;

namespace Protocol {
  GPSDataMessage parseSerialInputForGPS(string input);
  double parseLatitudeOrLongitude(string input);
}

#endif
