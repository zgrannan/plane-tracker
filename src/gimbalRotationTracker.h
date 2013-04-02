namespace PlaneTracking {
  class GimbalRotationTracker: public RawDataReceiver {
    public GimbalRotationTracker(SerialConnectionData connection,
                                 SerialReadProtocol<GimbalRotation> protocol);
  };
}
