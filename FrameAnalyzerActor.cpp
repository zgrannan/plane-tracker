#include <Theron/Theron.h>
#include "FrameAnalyzerActor.h"
#include "Vision.h"
#include "Messages.h"

#define CAMERA_H_FOV 60.0
#define CAMERA_V_FOV 45.0

using namespace Vision;

void FrameAnalyzerActor::Handler(const ImageMessage& message, const Theron::Address from){
  const RelativePositionMessage positionMessage = calculateRelativePosition(message);
  cerr << "Relative position calculated\n";
  Send(positionMessage, multimodalActor);
  cerr << "Position message sent\n";
}

RelativePositionMessage FrameAnalyzerActor::calculateRelativePosition(const ImageMessage& message){
  vector<PlaneVisionMessage> previousPlanes;
  vector<int> skyHSV;
  vector<int> planeHSV;

  PlaneVisionMessage data = findPlane(message.image,previousPlanes,skyHSV,planeHSV);
  double pan,tilt;
  if (data.hasPlane){
    previousPlanes.push_back(data);
    double dx = data.getDisplacement()[0];
    double dy = data.getDisplacement()[1];
    double centerX = data.result->width / 2;
    double centerY = data.result->height/ 2;
    pan = dx / data.result->width * CAMERA_H_FOV;
    tilt = dy / data.result->height * CAMERA_V_FOV;
    CvPoint origin = cvPoint(centerX,centerY);
    CvPoint destination =  cvPoint(centerX+dx,centerY+dy);
    CvScalar color = cvScalar(0,0,255);
    cvLine(message.image,origin,destination,color,2);
  }
  Send(data,imageViewer);
  if (data.hasPlane){
    return RelativePositionMessage(pan,tilt);
  } else {
    return RelativePositionMessage();
  }
}
