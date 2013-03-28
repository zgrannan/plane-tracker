#pragma once

namespace PlaneTracking {
  struct PlaneVisionMessage;
  struct GimbalInstruction;
  struct PlaneGPSMessage;
  struct TrackerGPSMessage;
  struct RSSIMessage;

  /**
   * This class listens to updates from the `PlaneTrackerBackend` and modifies the UI
   * with that data. It also sends the images to the flight recorder.
   */
  class TrackingDataUpdateListener{ 
  public:
    void updatePlaneImage(PlaneVisionMessage message);
    void updateCurrentInstruction(GimbalInstruction instruction);
    void updatePlaneGPS(PlaneGPSMessage planeGPS);
    void updateTrackerGPS(TrackerGPSMessage planeGPS);
    void updatePlaneRSSI(RSSIMessage planeRSSI);
  private:
    shared_ptr<UI> ui;
    shared_ptr<FlightRecorder> recorder;
  };
}
