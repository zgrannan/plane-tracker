namespace PlaneTracking {
  class TrackingMode;
  class GimbalCommander {
    public:
      GimbalCommander(DataAggregatorActor aggregator, SerialConnectionData connectionData);
      void sendCommandUsingMode(TrackingMode mode);
    private:
      void sendCommand(GimbalCommand command);
      const unique_ptr<GimbalCommandProtocol protocol>;
      const SerialConnection connection;
      const unique_ptr<CommandStation> commandStation;
  }
}
