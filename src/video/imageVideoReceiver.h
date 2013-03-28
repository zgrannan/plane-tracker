#pragma once
#include "src/video/videoReceiverInterface.h"
namespace PlaneTracking {
  class ImageVideoReceiver: VideoReceiverInterface {
  public:
    ImageVideoReceiver(std::string imageFilename);
  };
}
