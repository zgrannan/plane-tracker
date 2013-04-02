#pragma once

#include "src/commanding/commanders/interface.h"
#include "src/commanding/gimbalCommander.h"

namespace PlaneTracking {
  class DataAggregator;
  class VideoGPSNoRSSICommander : public VideoAvailable,
                                       GPSAvailable,
                                       GimbalCommander {
    VideoGPSNoRSSICommander(shared_ptr<DataAggregator> aggregator);
  };
}
