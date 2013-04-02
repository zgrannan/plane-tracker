#pragma once

#include "src/commanding/commanders/interface.h"
#include "src/commanding/gimbalCommander.h"

namespace PlaneTracking {
  class RSSIOnlyCommander : public RSSIAvailable,
                                  GimbalCommander {
    RSSIOnlyCommander(shared_ptr<DataAggregator> aggregator);
  };
}

