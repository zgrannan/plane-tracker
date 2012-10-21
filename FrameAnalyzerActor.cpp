#include "FrameAnalyzerActor.h"
#include <Theron/Theron.h>
#include "vision.h"

void FrameAnalyzerActor::Handler(const ImageMessage* message, const Theron::Address sender){
  RelativePositionMessage positionMessage = calculateRelativePosition(message);
  Send(positionMessage, multimodalActor);
}

RelativePositionMessage calculateRelativePosition(ImageMessage* message){
  vector<VisualPlaneData*>* previousPlanes = new vector<VisualPlaneData*>();
  vector<int>* skyHSV= new vector<int>();
  vector<int>* planeHSV = new vector<int>();

  VisualPlaneData* data = findPlane(image,previousPlanes,skyHSV,planeHSV);
  double tilt = data->getDisplacement()[0];
  double plan = data->getDisplacement()[1];
  return RelativePositionMessage(pan,tilt);
}
