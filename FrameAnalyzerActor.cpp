#include <Theron/Theron.h>
#include "FrameAnalyzerActor.h"
#include "Vision.h"
#include "Messages.h"

using namespace Vision;

void FrameAnalyzerActor::Handler(const ImageMessage& message, const Theron::Address from){
  const RelativePositionMessage positionMessage = calculateRelativePosition(message);
  Send(positionMessage, multimodalActor);
}

RelativePositionMessage FrameAnalyzerActor::calculateRelativePosition(const ImageMessage& message){
  vector<VisualPlaneData> previousPlanes;
  vector<int> skyHSV;
  vector<int> planeHSV;

  VisualPlaneData data = findPlane(message.image,previousPlanes,skyHSV,planeHSV);
  if (data.hasPlane){
    double tilt = data.getDisplacement()[0];
    double pan = data.getDisplacement()[1];
    return RelativePositionMessage(pan,tilt);
  } else {
    return RelativePositionMessage();
  }
}
