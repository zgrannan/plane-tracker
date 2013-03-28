#pragma once
#include <boost/optional/optional.hpp>

namespace PlaneTracking {
  
  struct GPSCoordinate;

  /**
   * This class is responsible for parsing strings for GPS data
   */
  class GPSProtocol {
    public:
      virtual boost::optional<GPSCoordinate> parse(std::string data) const = 0;
  }
}
