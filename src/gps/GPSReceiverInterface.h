#pragma once

namespace PlaneTracking {

  class GPSProtocol;
  class SerialConnection;

  class GPSReceiverInterface : public RawDataReceiver {
    public:
      GPSReceiverInterface(shared_ptr<GPSProtocol> protocol,
                           shared_ptr<SerialConnection> serial);
    
    protected:
      static virtual Message<GPSCoordinate> toMessage(GPSCoordinate coordinate) const = 0;
      shared_ptr<GPSProtocol> protocol;
      shared_ptr<SerialConnection> serial;
  };
}
