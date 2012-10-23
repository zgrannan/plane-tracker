#include <Theron/Theron.h>
#include "FrameAnalyzerActor.h"
#include "Vision.h"
#include "Messages.h"

using namespace Vision;

void FrameAnalyzerActor::Handler(const ImageMessage& message, const Theron::Address sender){
  RelativePositionMessage positionMessage = this->calculateRelativePosition(message);
  Send(positionMessage, multimodalActor);
}

RelativePositionMessage FrameAnalyzerActor::calculateRelativePosition(const ImageMessage& message){
  vector<VisualPlaneData*>* previousPlanes = new vector<VisualPlaneData*>();
  vector<int>* skyHSV= new vector<int>();
  vector<int>* planeHSV = new vector<int>();

  VisualPlaneData* data = findPlane(message.image,previousPlanes,skyHSV,planeHSV);
  double tilt = data->getDisplacement()[0];
  double pan = data->getDisplacement()[1];
  return RelativePositionMessage(pan,tilt);
}
