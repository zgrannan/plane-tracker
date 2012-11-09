#include <Theron/Theron.h>
#include "FrameAnalyzerActor.h"
#include "Vision.h"
#include "Messages.h"
#include "Log.h"

#define CAMERA_H_FOV 160//12.8
#define CAMERA_V_FOV 145//8.6

void FrameAnalyzerActor::Handler(const ImageMessage& message, const Theron::Address){
  if (GetNumQueuedMessages() > 1 || disabled){
    IplImage* image = message.image;
    cvReleaseImage(&image);
    return;
  }
  const RelativePositionMessage positionMessage = calculateRelativePosition(message);
  Log::debug("Relative position calculated");
  Send(positionMessage, multimodalActor);
  Log::debug("Position message sent");
}

RelativePositionMessage FrameAnalyzerActor::calculateRelativePosition(const ImageMessage& message){
  vector<PlaneVisionMessage> previousPlanes;

  PlaneVisionMessage data = vision->findPlane(message.image,previousPlanes);
  double pan = 0,tilt = 0;
  if (data.hasPlane){
    //previousPlanes.push_back(data);
    double dx = data.getDisplacement()[0];
    double dy = data.getDisplacement()[1];
    double centerX = data.result->width / 2;
    double centerY = data.result->height/ 2;
    pan = dx / data.result->width * CAMERA_H_FOV;
    tilt = dy / data.result->height * CAMERA_V_FOV;
    if (drawLine) {
      Log::debug("Drawing line");
      CvPoint origin = cvPoint(centerX,centerY);
      CvPoint destination =  cvPoint(centerX+dx,centerY+dy);
      CvScalar color = cvScalar(0,0,255);
      cvLine(message.image,origin,destination,color,2);
    }
  }
  Send(data,imageViewer);
  if (data.hasPlane){
    return RelativePositionMessage(pan,tilt);
  } else {
    return RelativePositionMessage();
  }
}
