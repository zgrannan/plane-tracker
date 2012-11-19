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
      bool drawLine,
      Vision* vision,
      Theron::Address imageViewer,
      Theron::Address multimodalActor) :
        Theron::Actor(framework),
        drawLine(drawLine),
        vision(vision),
        imageViewer(imageViewer),
        multimodalActor(multimodalActor){
          RegisterHandler(this, &FrameAnalyzerActor::Handler);
        }
      void disable(){ disabled = true; }
      void enable(){ disabled = false; }
      void setEdgeThresholding(int thresholding){ vision->setEdgeThresholding(thresholding); }
      void setMinBlobSize(int blobSize){ vision->setMinBlobSize(blobSize); }
      void setMaxBlobSize(int blobSize){ vision->setMaxBlobSize(blobSize); }

  private:
    bool drawLine;
    Vision* vision;
    Theron::Address imageViewer;
    Theron::Address multimodalActor;
    bool disabled = false;
    list<PlaneVisionMessage> previousPlanes;

    void Handler(const ImageMessage& message, const Theron::Address from);
    RelativePositionMessage calculateRelativePosition(const ImageMessage& message);
};

#endif
