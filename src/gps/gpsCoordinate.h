#pragma once

namespace PlaneTracking {
  struct GPSCoordinate {
    double lat;
    double lon;
    Units::Meters altitude;
  }
}
