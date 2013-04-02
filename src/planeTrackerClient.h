namespace PlaneTracking {
  class PlaneTrackerClient {
    public:
      PlaneTrackerClient(PlaneTrackerArgs args)
      void start();
    private:
      option<UI> ui;
      PlaneTrackerBackend backend;
  };
}
