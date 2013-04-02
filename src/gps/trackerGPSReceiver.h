#pragma once

#include "src/gps/gpsReceiverInterface.h"

namespace PlaneTracking {
  class GPSProtocol;
  class SerialConnection;

  class TrackerGPSReceiver : public GPSReceiverInterface {
    TrackerGPSReceiver(shared_ptr<GPSProtocol> protocol,
                     shared_ptr<SerialConnection> serial);

  }
}
