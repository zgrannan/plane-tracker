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
  private:
    bool drawLine;
    Vision* vision;
    Theron::Address imageViewer;
    Theron::Address multimodalActor;
    bool disabled = false;

    void Handler(const ImageMessage& message, const Theron::Address from);
    RelativePositionMessage calculateRelativePosition(const ImageMessage& message);
};

#endif
