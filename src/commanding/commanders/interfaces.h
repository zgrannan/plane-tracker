#pragma once

namespace PlaneTracking {
  class VideoAvailable {
    protected:
      shared_ptr<Message<PlaneVisionResult>> visionMessage;
      shared_ptr<boost::optional<Message<PlaneVisionResult>>> lastVisionMessage;
  };

  class GPSAvailable {
    protected:
      shared_ptr<Message<PlaneGPSCoordinate>> planeGPSCoordinate;
      shared_ptr<Message<TrackerGPSCoordinate>> trackerGPSCoordinate;
  };

  class RSSIAvailable {
    protected:
      shared_ptr<Message<RSSIData>> rssiInfo;
  };
}
