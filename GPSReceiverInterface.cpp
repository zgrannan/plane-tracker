#include <Theron/Theron.h>
#include <iostream>
#include <boost/thread.hpp>
#include "GPSReceiverInterface.h"
#include "Messages.h"
#include "Protocol.h"


using namespace std;
using namespace Messages;

GPSReceiverInterface::GPSReceiverInterface(Theron::Framework& framework, Theron::Address georeferencingActor): georeferencingActor(georeferencingActor), framework(framework), io(), port(io,"/dev/ttys0"){
  boost::thread workerThread(&GPSReceiverInterface::workerFunction,this);
}

void GPSReceiverInterface::workerFunction(){
  boost::system::error_code error; 
  boost::asio::streambuf buffer; 
  istream str(&buffer); 
  string s; 
  GPSDataMessage message;
  while (true){
    boost::asio::read_until( port, buffer, "\r\n", error );
    getline(str,s);
    message = Protocol::parseSerialInputForGPS(s);
    sendGPSData(message);
  }
}

void GPSReceiverInterface::sendGPSData(GPSDataMessage message){
  framework.Send(message,receiver.GetAddress(),georeferencingActor); 
}
