#ifndef GPSRECIEVER_INTERFACE_H 
#define GPSRECIEVER_INTERFACE_H 

#include <boost/asio.hpp>
#include <Theron/Theron.h>

class GPSDataMessage;

class GPSRecieverInterface {
  GPSRecieverInterface(Theron::Framework framework, Theron::Address georeferencingActor);
private: 
  void workerFunction();
  void sendGPS(GPSDataMessage message);
  boost::asio::serial_port port;
  Theron::Address georeferencingActor;
  Theron::Framework framework;
};

#endif
