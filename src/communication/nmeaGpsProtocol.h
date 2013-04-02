#pragma once
#include <boost/optional/optional.hpp>
#include "src/gps/gpsProtocol.h"

namespace PlaneTracking {
  
  struct GPSCoordinate;
  /**
   * This class is responsible for parsing strings for GPS data
   */
  class NMEAGPSProtocol: GPSProtocol{};
}
