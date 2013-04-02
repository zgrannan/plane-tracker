#pragma once

#include "src/commanding/commanders/interface.h"
#include "src/commanding/gimbalCommander.h"

namespace PlaneTracking {
  class AllAvailableCommander : public VideoAvailable,
                                       GPSAvailable,
                                       RSSIAvailable,
                                       GimbalCommander {
    AllAvailableCommander(shared_ptr<DataAggregator> aggregator);
  };
}
