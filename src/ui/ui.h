#pragma once

namespace PlaneTracking {
  class PlaneTrackerBackend;
  class UiEventHandler;
  class TrackingDataUpdateListener;
  class TrackingModeUpdateListener;
  class FlightRecorder;
  
  /**
   * This class is responsible for rendering the UI and responding to user input.
   * It also exposes an interface for `PlaneTrackerClient` to update the UI
   */
  class UI {
  public: 

    /**
     * Starts the UI thread. Should return immediately.
     */
    void run();

  private:
    shared_ptr<TrackingDataUpdateListener> trackingDataUpdateListener;
    shared_ptr<TrackingModeUpdateListener> modeUpdateListener;
    shared_ptr<UiEventHandler> uiEventHandler;
    shared_ptr<PlaneTrackerBackend> backend;
    shared_ptr<FlightRecorder> recorder;
  };
}
