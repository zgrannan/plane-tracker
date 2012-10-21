#include <Theron/Theron.h>
#include "GPSRecieverInterface.h"
#include "Messages.h"
#include "Protocol.h"

GPSReceiverInterface::GPSRecieverInterface(Theron::Framework framework, Theron::Address georeferencingActor){
  this->georeferencingActor = georeferencingActor;
  this->framework = framework;
  port.open("/dev/ttys0");
  boost::thread workerThread(workerFunction);
}

void GPSRecieverInterface::workerThread(){
  boost::system::error_code error; 
  boost::asio::streambuf buffer; 
  istream str(&buffer); 
  string s; 
  GPSDataMessage message;
  while (true){
    boost::asio::read_until( *socket, buffer, "\n", error );
    getline(str,s);
    message = Protocol::parseSerialInputForGPS(s);
    sendGPSMessage(message);
}

void GPSRecieverInterface::sendGPSData(GPSDataMessage message){
  framework.send(message,reciever.getAddress(),georeferencingActor); 
}
