#pragma once

#include "src/commanding/commanders/interface.h"
#include "src/commanding/gimbalCommander.h"

namespace PlaneTracking {
  class DataAggregator;
  class VideoOnlyCommander : public VideoAvailable,
                                  GimbalCommander {
    VideoOnlyCommander(shared_ptr<DataAggregator> aggregator);
  };
}
