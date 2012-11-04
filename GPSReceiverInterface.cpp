#include <Theron/Theron.h>
#include <iostream>
#include <boost/thread.hpp>
#include <termios.h>
#include "GPSReceiverInterface.h"
#include "Messages.h"
#include "Protocol.h"
#include "Log.h"


using namespace std;
using namespace Messages;

GPSReceiverInterface::GPSReceiverInterface(Theron::Framework& framework, string serialPort, Theron::Address georeferencingActor): serialPort(serialPort), georeferencingActor(georeferencingActor), framework(framework){ 
  boost::thread workerThread(&GPSReceiverInterface::workerFunction,this);
}


void GPSReceiverInterface::workerFunction(){
  int fd = open(serialPort.c_str(),O_RDONLY | O_NOCTTY | O_NDELAY);
  termios options;
  tcgetattr(fd, &options);
  options.c_cflag |= (CLOCAL | CREAD);
  tcsetattr(fd,TCSANOW, &options);
  char* buffer = (char*)malloc(1024);
  string extra = "";
  GPSDataMessage message;
  while (true){
    read(fd,buffer,1024);
    if (*buffer != '\0'){
      try {
        message = Protocol::parseSerialInputForGPS(extra + string(buffer),extra);
        sendGPSData(message);
      } catch (string msg){
        Log::error("Protocol error: " + msg);
      }
    }
    *buffer = '\0';
    boost::this_thread::sleep(boost::posix_time::milliseconds(200)); 
  }
}

void GPSReceiverInterface::sendGPSData(const GPSDataMessage message){
  framework.Send(message,receiver.GetAddress(),georeferencingActor); 
}
