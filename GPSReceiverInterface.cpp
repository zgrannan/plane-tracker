#include <Theron/Theron.h>
#include <iostream>
#include <boost/thread.hpp>
#include <termios.h>
#include "GPSReceiverInterface.h"
#include "Messages.h"
#include "Protocol.h"


using namespace std;
using namespace Messages;

GPSReceiverInterface::GPSReceiverInterface(Theron::Framework& framework, string serialPort, Theron::Address georeferencingActor): serialPort(serialPort), georeferencingActor(georeferencingActor), framework(framework){ 
  boost::thread workerThread(&GPSReceiverInterface::workerFunction,this);
}


void GPSReceiverInterface::workerFunction(){
  int fd = open("/dev/ptyw0",O_RDONLY | O_NOCTTY | O_NDELAY);
  termios options;
  tcgetattr(fd, &options);
  options.c_cflag |= (CLOCAL | CREAD);
  tcsetattr(fd,TCSANOW, &options);
  char* buffer = (char*)malloc(1024);
  GPSDataMessage message;
  while (true){
    read(fd,buffer,1024);
    if (*buffer != '\0'){
      try {
        message = Protocol::parseSerialInputForGPS(string(buffer));
        sendGPSData(message);
      } catch (string msg){
        cerr <<"Protocol error: "<<msg<<endl;
      }
    }
    *buffer = '\0';
    boost::this_thread::sleep(boost::posix_time::milliseconds(200)); 
  }
}

void GPSReceiverInterface::sendGPSData(const GPSDataMessage message){
  framework.Send(message,receiver.GetAddress(),georeferencingActor); 
}
