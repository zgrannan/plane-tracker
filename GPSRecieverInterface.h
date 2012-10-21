#ifndef GPSRECIEVER_INTERFACE_H 
#define GPSRECIEVER_INTERFACE_H 

class boost::serial_port;
class GPSDataMessage;
class Theron::Address;
class GPSRecieverInterface: ListeningInterface {
  GPSRecieverInterface(Theron::Framework framework, Theron::Address georeferencingActor);
private: 
  void workerFunction();
  void sendGPS(GPSDataMessage message);
  boost::serial_port port;
  Theron::Address georeferencingActor;
  Theron::Framework framework;
};

#endif
