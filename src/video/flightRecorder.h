#pragma once

namespace PlaneTracking {
  class FlightRecorder {
    virtual void start() = 0;
    virtual void stop() = 0;
  }
}
