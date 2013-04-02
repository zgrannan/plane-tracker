#pragma once

#include "src/commanding/commanders/interface.h"
#include "src/commanding/gimbalCommander.h"

namespace PlaneTracking {
  class DataAggregator;
  class VideoRSSINoGPSCommander : public VideoAvailable,
                                       RSSIAvailable,
                                       GimbalCommander {
    VideoRSSINoGPSCommander(shared_ptr<DataAggregator> aggregator);
  };
}

