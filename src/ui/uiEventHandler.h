#pragma once

namespace PlaneTracking {

  class PlaneTrackerBackend;
  class FlightRecorder;

  /**
   * This class is responsible for handling input from the user to the UI, 
   * and notifying the backend accordingly
   */
  class UiEventHandler {
  public:

    /**
     * Sets the maximum and minimum bounding box size that each plane blob must
     * fit in
     */
    void setMaxBoxSize(int size);
    void setMinBoxSize(int size);

    /**
     * Sets the edge thresholding level for the CV algorithm
     */
    void setEdgeThresholdingLevel(int level);
    void toggleVideo(bool state);
    void togglePlaneGPS(bool state);
    void toggleTrackerGPS(bool state);
    void toggleRSSI(bool state);
    bool toggleRecording(bool state);
  private:
    shared_ptr<PlaneTrackerBackend> backend;
    shared_ptr<FlightRecorder> recorder;
  };
}
