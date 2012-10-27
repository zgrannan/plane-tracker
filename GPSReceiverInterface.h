#ifndef GPSRECIEVER_INTERFACE_H 
#define GPSRECIEVER_INTERFACE_H 

#include <boost/asio.hpp>
#include <Theron/Theron.h>
#include "Messages.h"

class GPSReceiverInterface {
public:
  GPSReceiverInterface(Theron::Framework& framework, Theron::Address georeferencingActor);
private: 
  void workerFunction();
  void sendGPSData(Messages::GPSDataMessage message);
  boost::asio::io_service io;
  boost::asio::serial_port port;
  Theron::Address georeferencingActor;
  Theron::Framework& framework;
  Theron::Receiver receiver;
};

#endif
