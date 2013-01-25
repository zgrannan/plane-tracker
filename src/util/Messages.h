#ifndef MESSAGES_H
#define MESSAGES_H
#include <cvblob.h>
#include <cv.h>

using namespace cv;
using namespace cvb;

namespace Messages{

  class DisableMessage;
  class EnableMessage;

  class AmplificationMessage{
    public:
      AmplificationMessage(int amplification): amplification(amplification){}
      const int amplification;
  };

  class ImageMessage{
    public:
      string name;
      IplImage* image;
      ImageMessage(IplImage* const image): name("unnamed"), image(image){}
      ImageMessage(const string name, IplImage* const image): name(name), image(image){}
  };

  class BlobPositionMessage{
    public:
      BlobPositionMessage(const double x, const double y): x(x), y(y) {}
      const double x,y;
  };

  /**
   * This class represents the result of a successful CV plane detection
   * planeBlob: The CvBlob that the plane has been identified as
   * image: The image that captured by the camera
   */
  class PlaneVisionMessage {
    public:
      CvBlob planeBlob;
      IplImage* result;
      vector<ImageMessage> extras;
      vector<int> getDisplacement() const;
      bool hasPlane;
      bool userHasConfirmed;
      PlaneVisionMessage(const CvBlob planeBlob, IplImage* const result, const vector<ImageMessage> extras):
        planeBlob(planeBlob),
        result(result),
        extras(extras),
        hasPlane(true),
        userHasConfirmed(false){}
      PlaneVisionMessage(const CvBlob planeBlob, IplImage* const result,
                         const vector<ImageMessage> extras, const bool userHasConfirmed):
        planeBlob(planeBlob),
        result(result),
        extras(extras),
        hasPlane(true),
        userHasConfirmed(userHasConfirmed){}
      PlaneVisionMessage(): 
        planeBlob(CvBlob()),
        result(0),
        extras(vector<ImageMessage>()),
        hasPlane(false),
        userHasConfirmed(false){}
      PlaneVisionMessage(IplImage* const image, const vector<ImageMessage> extras): 
        planeBlob(CvBlob()),
        result(image),
        extras(extras),
        hasPlane(false),
        userHasConfirmed(false){}
  };

  class GPSDataMessage{
    public:
      const double lat, lon, alt;
      const bool hasData;
      GPSDataMessage(const double lat, const double lon, const double alt):
        lat(lat),lon(lon),alt(alt),hasData(true){}
      GPSDataMessage(): lat(0),lon(0),alt(0),hasData(false){}
  };


  class PositionMessage{
    public:
      const double pan, tilt;
      const bool positionLost;
      PositionMessage(const double pan, const double tilt): pan(pan), tilt(tilt), positionLost(false){}
      PositionMessage(): pan(0), tilt(0), positionLost(true){}
      virtual vector<char> toBytes() const = 0;
  };

  class RelativePositionMessage: public PositionMessage{
    public:
      RelativePositionMessage(const double pan, const double tilt): PositionMessage(pan,tilt){}
      RelativePositionMessage(): PositionMessage(){}
      vector<char> toBytes() const;
  };

  class AbsolutePositionMessage: public PositionMessage{
    public:
      AbsolutePositionMessage(const double pan, const double tilt): PositionMessage(pan,tilt){}
      AbsolutePositionMessage(): PositionMessage(){}
      vector<char> toBytes() const;
  };

  class UseRSSIMessage: public PositionMessage{
    public:
      vector<char> toBytes() const;
  };
}

#endif
