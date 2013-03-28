#include <Theron/Theron.h>
#include "src/vision/FrameAnalyzerActor.h"
#include "src/vision/Vision.h"
#include "src/util/Messages.h"
#include "src/util/Log.h"
#include <cv.h>

#define CAMERA_H_FOV 12.8
#define CAMERA_V_FOV 8.6

using namespace cv;

void FrameAnalyzerActor::selectColor(){
  if (!hasLock){
    Log::warn("Cannot select color: frameAnalyzer does not have a lock.");
  } else{
    vision->setUseColor(true);
  }
}

void FrameAnalyzerActor::deselectColor(){
  DEBUG("No longer using plane color");
  vision->setUseColor(false);
}

void FrameAnalyzerActor::BlobPositionHandler(const BlobPositionMessage& message,
                                             const Theron::Address){
  previousPlanes = list<PlaneVisionMessage>();
  vision->setPlaneBlob(message.x, message.y);
} 

void FrameAnalyzerActor::ImageHandler(const ImageMessage& message, const Theron::Address){
  if (GetNumQueuedMessages() > 1){
    DEBUG("Skipped a frame before analysis");
    cvReleaseImage((IplImage**)&(message.image));
    return;
  }
  const RelativePositionMessage positionMessage = calculateRelativePosition(message);
  DEBUG("Relative position calculated");
  if (!disabled){
    Send(positionMessage, multimodalActor);
    DEBUG("Position message sent");
  } else {
    Send(RelativePositionMessage(),multimodalActor);
  }
}

RelativePositionMessage FrameAnalyzerActor::calculateRelativePosition(const ImageMessage& message){
  IplImage* image;
  if ( scale != 1.0){
    image = cvCreateImage(cvSize(message.image->height * scale,
                                       message.image->width * scale),
                                       message.image->depth,
                                       message.image->nChannels);
    cvResize(message.image,image);
    cvReleaseImage((IplImage**)&(message.image));
  } else {
    image = message.image;
  }

  PlaneVisionMessage data = vision->findPlane(image,previousPlanes);

  if (data.userHasConfirmed){
    Log::log("Plane found from blob");
    previousPlanes = list<PlaneVisionMessage>();
    for (unsigned int i = 0; i < planesToConsider; i ++){
      previousPlanes.push_front(data);
    }
  }

  if (sizeBoxToDraw.isDefined()){
    DEBUG("Drawing sizeBox");
    double size = ((double)*sizeBoxToDraw) / 100.0 * image->height;
    CvPoint ul = cvPoint(5,5);
    CvPoint br = cvPoint(5 + size, 5 + size);
    CvScalar color = cvScalar(0,0,255);
    cvRectangle(image,ul,br,color);
    sizeBoxToDraw = None<int>();
  }

  double pan = 0,tilt = 0;
  if (data.hasPlane){
    consecutiveLost = 0;
    previousPlanes.push_front(data);
    if ( previousPlanes.size() > planesToConsider ) {
      previousPlanes.pop_back();
    }
    double dx = data.getDisplacement()[0];
    double dy = data.getDisplacement()[1];
    double centerX = data.result->width / 2;
    double centerY = data.result->height/ 2;
    pan = dx / data.result->width * CAMERA_H_FOV;
    tilt = dy / data.result->height * CAMERA_V_FOV;
    if (drawLine) {
      DEBUG("Drawing line");
      CvPoint origin = cvPoint(centerX,centerY);
      CvPoint destination =  cvPoint(centerX+dx,centerY-dy);
      CvScalar color = cvScalar(0,0,255);
      cvLine(image,origin,destination,color,2);
    }
  } else if (previousPlanes.size() > 0) {
    consecutiveLost++;
    if ( consecutiveLost > 5){
      previousPlanes.pop_back();
    }
  } 
  
  Send(data,imageViewer);
  if (data.hasPlane){
    hasLock = true;
    return RelativePositionMessage(pan,tilt);
  } else {
    hasLock = false;
    return RelativePositionMessage();
  }
}
