#include <Theron/Theron.h>
#include "FrameAnalyzerActor.h"
#include "Vision.h"
#include "Messages.h"

#define CAMERA_H_FOV 60.0
#define CAMERA_V_FOV 45.0

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
    previousPlanes.push_back(data);
    double dx = data.getDisplacement()[0];
    double dy = data.getDisplacement()[1];
    double centerX = data.image->width / 2;
    double centerY = data.image->height/ 2;
    double pan = dx / data.image->width * CAMERA_H_FOV;
    double tilt = dy / data.image->height * CAMERA_V_FOV;
    CvPoint origin = cvPoint(centerX,centerY);
    CvPoint destination =  cvPoint(centerX+dx,centerY+dy);
    CvScalar color = cvScalar(0,0,255);
    cvLine(message.image,origin,destination,color,2);
    Send(message,imageViewer);
    return RelativePositionMessage(pan,tilt);
  } else {
    return RelativePositionMessage();
  }
}
