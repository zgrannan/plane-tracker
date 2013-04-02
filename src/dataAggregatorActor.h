#pragma once

namespace PlaneTracking {
  class DataAggregatorActor : public Theron::Actor {
    friend struct CommandStation;   

    public:
      Trackingmode getCurrentTrackingMode();

    private: 
      unique_ptr<Message<PlaneVisionResult>> visionMessage;
      unique_ptr<Message<PlaneVisionResult>> lastVisionMessage;
      unique_ptr<Message<RSSIData>> rssiData;
      unique_ptr<Message<PlaneGPSCoordinate>> planeLocation;
      unique_ptr<Message<TrackerGPSCoordinate>> trackerLocation;

      const MilliSeconds videoTimeout;
      const MilliSeconds planeGPSTimeout;
      const MilliSeconds rssiTimeout;

      unique_ptr<GimbalCommander> commander;
      shared_ptr<PlaneTrackerBackend> backend;
  }
}
