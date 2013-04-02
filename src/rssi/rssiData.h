#pragma once

class PlaneTracking {

  namespace Units {
    enum Direction;
  }
  
  struct RSSIData {
    double raw;
    Units::Direction direction;
  };
}
