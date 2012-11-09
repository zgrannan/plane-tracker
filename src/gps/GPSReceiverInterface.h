#ifndef GPSRECIEVER_INTERFACE_H 
#define GPSRECIEVER_INTERFACE_H 

#include <boost/asio.hpp>
#include <Theron/Theron.h>
#include "src/util/Messages.h"

class GPSReceiverInterface {
public:
  GPSReceiverInterface(Theron::Framework& framework, string serialPort, Theron::Address georeferencingActor);
private: 
  void workerFunction();
  void sendGPSData(Messages::GPSDataMessage message);

  Theron::Framework& framework;
  string serialPort;
  Theron::Address georeferencingActor;
  Theron::Receiver receiver;
};

#endif
