#include <Theron/Theron.h>
#include "src/vision/FrameAnalyzerActor.h"
#include "src/vision/Vision.h"
#include "src/util/Messages.h"
#include "src/util/Log.h"

#define CAMERA_H_FOV 12.8
#define CAMERA_V_FOV 8.6

void FrameAnalyzerActor::Handler(const ImageMessage& message, const Theron::Address){
  if (GetNumQueuedMessages() > 1){
    cvReleaseImage((IplImage**)&(message.image));
    return;
  }
  const RelativePositionMessage positionMessage = calculateRelativePosition(message);
  Log::debug("Relative position calculated");
  if (!disabled){
    Send(positionMessage, multimodalActor);
    Log::debug("Position message sent");
  } else {
    Send(RelativePositionMessage(),multimodalActor);
  }
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
      CvPoint destination =  cvPoint(centerX+dx,centerY-dy);
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
