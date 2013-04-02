#pragma once
#include <boost/optional/optional.hpp>

namespace PlaneTracking {
  
  struct GPSCoordinate;

  /**
   * This class is responsible for parsing strings for GPS data
   */
  class GPSProtocol : SerialReadProtocol<GPSCoordinate> {}
}
