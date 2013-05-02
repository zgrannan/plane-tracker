#include <boost/lexical_cast.hpp>
#include "src/main/MultimodalActor.h"
#include "src/gps/Protocol.h"
#include "src/util/Messages.h"
#include "src/util/Log.h"

using namespace boost;
using namespace boost::posix_time;

MultimodalActor::MultimodalActor(Theron::Framework &framework, 
                                 const string serialPort,
                                 const int baudRate): 
  Theron::Actor(framework),
  useRSSI(true),
  videoLost(true),
  gpsLost(true){
    DEBUG("Multimodal actor opening serial port: " +serialPort);
    fd = open(serialPort.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    termios options;
    tcgetattr(fd, &options);
    cfsetispeed(&options,baudRate);
    cfsetospeed(&options,baudRate);
    options.c_cflag |= (CLOCAL | CREAD);
    tcsetattr(fd,TCSANOW, &options);
    RegisterHandler(this, &MultimodalActor::GPSHandler);
    RegisterHandler(this, &MultimodalActor::VisionHandler);
  }
void MultimodalActor::GPSHandler(const AbsolutePositionMessage &message,
    const Theron::Address){
  
  DEBUG("AbsolutePositionMessage received");
  if (!message.positionLost){
    lastGPSMessage = message;
    gpsLost = false;
    useRSSI = false;
    DEBUG("Pan: " + lexical_cast<string>(message.pan) + " Tilt: " + lexical_cast<string>(message.tilt));
    if (videoLost)
      instructGimbal(message);
  } else {
    DEBUG("GPS Lost");
    gpsLost = true;
    if (videoLost == true && useRSSI == false){
      DEBUG("Switching to RSSI");
      useRSSI = true;
      instructGimbal(UseRSSIMessage());
    }
  }
}


void MultimodalActor::VisionHandler(const RelativePositionMessage &message,
    const Theron::Address){
  DEBUG("RelativePositionMessage received");
  if (!message.positionLost && !lastGPSMessage.positionLost){
    if ( (message.pan > 0 ^ lastGPSMessage.pan > 0) ||
         (message.tilt > 0 ^ lastGPSMessage.tilt > 0 )){
      Log::warn("GPS and Video contradict each other. You should probably disable one.");
    }
  }
  lastVisionMessage = message;
  if (!message.positionLost){
    videoLost = false;
    useRSSI = false;
    DEBUG("Pan: " + lexical_cast<string>(message.pan) + " Tilt: " + lexical_cast<string>(message.tilt));
    if (amplification != 1.0) {
      const auto amplifiedMessage = RelativePositionMessage(message.pan * amplification,
          message.tilt * amplification);
      instructGimbal(amplifiedMessage);
    } else {
      instructGimbal(message);
    }
  } else {
    DEBUG("Video Not Available");
    videoLost = true;
    if (videoLost == true && useRSSI == false){
      DEBUG("Switching to RSSI");
      useRSSI = true;
      instructGimbal(UseRSSIMessage());
    }
  }
}

void MultimodalActor::instructGimbal(const PositionMessage &message){
  auto now = microsec_clock::local_time();
  if ((now - lastInstructionTime).total_milliseconds() < 100){
    DEBUG("Not instructing gimbal, last instruction was less than 100ms ago");
    return;
  } 
  const vector<char> bytes = message.toBytes();
  const char* const bytePtr = &bytes[0];
  DEBUG(string("Writing message: ") + string(bytes.begin(),bytes.end()));
  write(fd, bytePtr, bytes.size());
  lastInstructionTime = now;
}

void MultimodalActor::setAmplification(const double amplification){
  this->amplification = amplification;
}
