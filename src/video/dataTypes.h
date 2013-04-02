#pragma once

#include <cv.h>
#include <cvblob.h>
#include <boost/optional/optional.hpp>
#include "src/units/size.h"

namespace PlaneTracking {
  namespace Units {
    struct Degrees;
  }
  struct ColorHSV {
    double hue, saturation, value;
  };

  struct VisionResult {
    cv::Mat image;
    boost::optional<cvb::CvBlob> plane;
    std::vector<cv::Mat> extras;
  };

  struct CameraArguments {
    Units::Degrees horizontalFOV, verticalFOV;
    Units::Size<Pixels> imageDimensions;
  };
}
