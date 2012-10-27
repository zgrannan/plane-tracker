#include <Theron/Theron.h>
#include <iostream>
#include <boost/thread.hpp>
#include "GPSReceiverInterface.h"
#include "Messages.h"
#include "Protocol.h"


using namespace std;
using namespace Messages;

GPSReceiverInterface::GPSReceiverInterface(Theron::Framework& framework, Theron::Address georeferencingActor): georeferencingActor(georeferencingActor), framework(framework){ 
  boost::thread workerThread(&GPSReceiverInterface::workerFunction,this);
}


void GPSReceiverInterface::workerFunction(){
  FILE* fd = fopen("/dev/ttyw0","r");
  char* buffer = (char*)malloc(1024);
  GPSDataMessage message;
  while (true){
    fgets(buffer,1024,fd);
    message = Protocol::parseSerialInputForGPS(string(buffer));
    sendGPSData(message);
  }
}

void GPSReceiverInterface::sendGPSData(GPSDataMessage message){
  framework.Send(message,receiver.GetAddress(),georeferencingActor); 
}
