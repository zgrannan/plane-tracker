#pragma once

namespace PlaneTracking {
  namespace Commanders {
    class AllAvailableCommander;
    class VideoGPSNoRSSICommander;
    class VideoRSSINoGPSCommander;
    class VideoCommander;
    class GPSCommander;
    class RSSICommander;
    class NoCommander;
    class UICommander;
   }
  class GimbalCommander;
  enum TrackingMode;

  struct CommandStation {
    CommandStation(AggregatorActor aggregator);
    GimbalCommander getCommanderForMode(TrackingMode);
    private:
      unique_ptr<Commanders::AllAvailableCommander> allAvailableCommander;
      unique_ptr<Commanders::VideoGPSNoRSSICommander> videoGPSNoRSSICommander;
      unique_ptr<Commanders::VideoRSSINoGPSCommander> videoRSSINoGPSCommander;
      unique_ptr<Commanders::VideoCommander> videoCommander;
      unique_ptr<Commanders::GPSCommander> gpsCommander;
      unique_ptr<Commanders::RSSICommander> rssiCommander;
      unique_ptr<Commanders::NoCommander> noCommander;
      unique_ptr<Commanders::UICommander> uiCommander;
  }
}
