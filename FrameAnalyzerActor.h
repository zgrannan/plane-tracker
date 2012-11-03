#ifndef FRAME_ANALYZER_ACTOR_H
#define FRAME_ANALYZER_ACTOR_H

#include <Theron/Theron.h>
#include "Messages.h"
#include "Vision.h"
#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace Messages;

class FrameAnalyzerActor : public Theron::Actor {
  public:
    FrameAnalyzerActor(Theron::Framework &framework,bool drawLine,Vision* vision, Theron::Address imageViewer, Theron::Address multimodalActor) :
      Theron::Actor(framework),
      drawLine(drawLine),
      vision(vision),
      imageViewer(imageViewer),
      multimodalActor(multimodalActor){
        RegisterHandler(this, &FrameAnalyzerActor::Handler);
      }
  private:
    Vision* vision;
    bool drawLine;
    RelativePositionMessage calculateRelativePosition(const ImageMessage& message);
    void Handler(const ImageMessage& message, const Theron::Address from);
    Theron::Address multimodalActor;
    Theron::Address imageViewer;
};

#endif
