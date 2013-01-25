#ifndef FRAME_ANALYZER_ACTOR_H
#define FRAME_ANALYZER_ACTOR_H

#include <Theron/Theron.h>
#include "src/util/Messages.h"
#include "src/vision/Vision.h"
#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace Messages;

class FrameAnalyzerActor : public Theron::Actor {
  public:
    FrameAnalyzerActor(
      Theron::Framework &framework,
      double scale,
      const unsigned int planesToConsider,
      bool drawLine,
      Vision* vision,
      Theron::Address imageViewer,
      Theron::Address multimodalActor) :
        Theron::Actor(framework),
        scale(scale),
        planesToConsider(planesToConsider),
        drawLine(drawLine),
        vision(vision),
        imageViewer(imageViewer),
        multimodalActor(multimodalActor){
          RegisterHandler(this, &FrameAnalyzerActor::ImageHandler);
          RegisterHandler(this, &FrameAnalyzerActor::BlobPositionHandler);
        }
      void disable(){ disabled = true; hasLock = false; }
      void enable(){ disabled = false; }
      void selectColor();
      void deselectColor();

      void setEdgeThresholding(int thresholding){ vision->setEdgeThresholding(thresholding); }
      void setMinBlobSize(int blobSize){ vision->setMinBlobSize(blobSize); }
      void setMaxBlobSize(int blobSize){ vision->setMaxBlobSize(blobSize); }
      void setPositionThresh(int thresh) { vision->setPositionThresh(thresh); }
      void setSizeThresh(int thresh) { vision->setSizeThresh(thresh); }
      void setColorThresh(int thresh) { vision->setColorThresh(thresh); }

  private:
    double scale = 1.0; 
    const unsigned int planesToConsider;
    bool drawLine;
    Vision* vision;
    Theron::Address imageViewer;
    Theron::Address multimodalActor;
    bool disabled = true;
    bool hasLock = false;
    unsigned int consecutiveLost = 0;
    list<PlaneVisionMessage> previousPlanes;

    void ImageHandler(const ImageMessage& message, const Theron::Address from);
    void BlobPositionHandler(const BlobPositionMessage& message, const Theron::Address from);
    RelativePositionMessage calculateRelativePosition(const ImageMessage& message);
};

#endif
