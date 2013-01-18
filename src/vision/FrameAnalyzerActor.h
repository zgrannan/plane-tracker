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
      bool drawLine,
      Vision* vision,
      Theron::Address imageViewer,
      Theron::Address multimodalActor) :
        Theron::Actor(framework),
        scale(scale),
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
      void setPositionWeight(int weight) { vision->setPositionWeight(weight); }
      void setRatioWeight(int weight) { vision->setRatioWeight(weight); }
      void setSizeWeight(int weight) { vision->setSizeWeight(weight); }
      void setColorWeight(int weight) { vision->setColorWeight(weight); }

  private:
    double scale = 1.0; 
    bool drawLine;
    Vision* vision;
    Theron::Address imageViewer;
    Theron::Address multimodalActor;
    bool disabled = true;
    bool hasLock = false;
    list<PlaneVisionMessage> previousPlanes;

    void ImageHandler(const ImageMessage& message, const Theron::Address from);
    void BlobPositionHandler(const BlobPositionMessage& message, const Theron::Address from);
    RelativePositionMessage calculateRelativePosition(const ImageMessage& message);
};

#endif
