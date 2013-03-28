#pragma once

namespace PlaneTracking {
  /**
   * This class receives updates from `PlaneTrackerBackend` when tracking modes
   * become available or unavailable. It modifies the UI accordingly
   */
  class TrackingModeUpdateListener {
  public:
    bool planeGPSLost();
    bool trackerGPSLost();
    bool planeNotVisible();
    bool rssiNotResponsive();
    bool planeGPSAvailable();
    bool trackerGPSUnavailable();
    bool planeVisible();
    bool rssiAvailable();
  private:
    shared_ptr<UI> ui;
  };
}
