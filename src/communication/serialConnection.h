#pragma once

namespace PlaneTracking {

  struct SerialConnectionData;
  /**
   * `RawDataReceivers` that use serial communication must extend this class. It opens the
   * serial connection, exposing a file descriptor that can be used for 
   * the serial communication.
   */
  class SerialConnection {
    public:
      SerialConnection(SerialConnectionData connectionInfo);
      void read(const int bytes);
      void write(string message);
    private:
      const int serialConnectionFileDescriptor;
  };
}
