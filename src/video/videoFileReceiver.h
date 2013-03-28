#pragma once
#include "src/video/videoReceiverInterface.h"
namespace PlaneTracking {
  class VideoFileReceiver: VideoReceiverInterface {
  public:
    VideoFileReceiver(std::string videoFilename);
  };
}
