#pragma once

#include "boost/optional/optional.hpp"

namespace PlaneTracking {
  
  namespace Units {
    class Meters;
    class Pixels;
  }

  namespace Georeference {
    Units::Size<Pixels> estimatePlaneSize(Size<Meters> planeSize,
                                          TrackerGPSCoordinate tracker,
                                          PlaneGPSCoordinate plane,
                                          CameraParameters camera);

    boost::optional<Units::Point2D<Pixels>> esimatePlaneLocation(TrackerGPSCoordinate tracker,
                                                PlaneGPSCoordinate plane,
                                                CameraParameters camera);

    bool planeInFov(TrackerGPSCoordinate tracker,
                    PlaneGPSCoordinate plane,
                    CameraParameters camera)

    bool contradicts(RSSIData rssiData, GPSCoordinate coordinate);
    bool shouldConsiderBlob(cvb::CvBlob planeBlob, Direction dir, CameraParameters camera);
  }
}
