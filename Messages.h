#ifndef MESSAGES_H
#define MESSAGES_H
#include <cv.h>

using namespace cv;

namespace Messages{

  class ImageMessage{
    public:
      IplImage* image;
      ImageMessage(IplImage* image): image(image){}
  };

  class GPSDataMessage{
    public:
      double lat, lon, alt;
      GPSDataMessage(double lat, double lon, double alt): lat(lat),lon(lon),alt(alt){}
      GPSDataMessage(): lat(0),lon(0),alt(0){}
  };


  class PositionMessage{
    public:
      double pan, tilt;
      bool positionLost;
      PositionMessage(double pan, double tilt): pan(pan), tilt(tilt), positionLost(false){}
      PositionMessage(): pan(0), tilt(0), positionLost(true){}
      virtual vector<char> toBytes() const = 0;
  };

  class RelativePositionMessage: public PositionMessage{
    public:
      RelativePositionMessage(double pan, double tilt): PositionMessage(pan,tilt){}
      RelativePositionMessage(): PositionMessage(){}
      vector<char> toBytes() const;
  };

  class AbsolutePositionMessage: public PositionMessage{
    public:
      AbsolutePositionMessage(double pan, double tilt): PositionMessage(pan,tilt){}
      AbsolutePositionMessage(): PositionMessage(){}
      vector<char> toBytes() const;
  };
}

#endif
