#ifndef GPSRECIEVER_INTERFACE_H 
#define GPSRECIEVER_INTERFACE_H 

#include <boost/asio.hpp>
#include <Theron/Theron.h>
#include "src/util/Messages.h"

class GPSReceiverInterface {
public:
  GPSReceiverInterface(Theron::Framework& framework,
                       const string serialPort,
                       const int baudRate,
                       const Theron::Address georeferencingActor);
private: 
  void workerFunction();
  void sendGPSData(Messages::GPSDataMessage message);

  Theron::Framework& framework;
  const string serialPort;
  const int baudRate;
  const Theron::Address georeferencingActor;
  const Theron::Receiver receiver;
};

#endif
