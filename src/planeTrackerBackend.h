namespace PlaneTracking {
  class PlaneTrackerBackend {
  public:
    PlaneTrackerBackend (PlaneTrackerBackedArgs arguments)
    void commandUsing(TrackingMode mode);

  private:
    const boost::optional<VideoReceiverInterface> videoReceiver;
    const boost::optional<PlaneGPSReceiver> planeGPSReceiver;
    const boost::optional<TrackerGPSReceiver> trackerGPSReceiver;
    const boost::optional<RSSIReceiver> rssiReceiver;
    const boost::optional<GimbalCommander> commander;
    const boost::optional<PlaneTrackerClient> client;
    DataAggregator aggregator;
  }
}
