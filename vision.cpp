#include <cvblob.h>
#include <cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include "Vision.h"
#include "Log.h"

/* Plane detection flags */
#define USE_VELOCITY true
#define USE_POSITION true
#define USE_PLANE_SIZE true
#define USE_SKY_COLOR true
#define USE_PLANE_COLOR true

/* Color -> Black and White conversion technique */
#define ADAPTIVE_THRESHHOLD 0

using namespace cvb;
using namespace std;
using namespace cv;

vector<double> Vision::getVelocityVector(CvBlob currentBlob, CvBlob lastBlob) {
  double dx = currentBlob.centroid.x - lastBlob.centroid.x;
  double dy = currentBlob.centroid.y - lastBlob.centroid.y;
  vector<double> result;
  result.push_back(dx);
  result.push_back(dy);
  return result;
}

IplImage* Vision::canny(IplImage* grayImage, vector<ImageMessage> &extras){
  int lowThreshold = 50;
  int ratio = 3;
  int kernelSize =3;
  Mat edges, dest;
  blur(Mat(grayImage), edges, Size(3,3));
  Canny(edges, edges, lowThreshold,lowThreshold * ratio, kernelSize);
  dest = Scalar::all(0);
  Mat(grayImage).copyTo(dest, edges);
  IplImage* result = cvCreateImage(cvGetSize(grayImage),8,1);
  cvThreshold(new IplImage(dest),result,1,255,CV_THRESH_BINARY);
  if (intermediateSteps)
    extras.push_back(ImageMessage("Edge detection", cvCloneImage(result)));
  
  return result;
}

/**
 * Converts a full-color image to a black and white image that can be used
 * for blob detection.
 */
IplImage* Vision::fullColorToBW (IplImage* image, int conversionMethod, vector<ImageMessage> &extras){
    assert(image != nullptr);
    IplImage* grayscaleImage = cvCreateImage(cvGetSize(image),8,1);
    IplImage* hsvImage = cvCreateImage(cvGetSize(image),image->depth,image->nChannels);

    // Create a hue-saturation-value version of this image
    cvCvtColor(image,hsvImage,CV_BGR2HSV);

    // Isolate the hue channel
    cvSplit(hsvImage,grayscaleImage,NULL,NULL,NULL);

    cvReleaseImage(&hsvImage);
    if (intermediateSteps)
      extras.push_back(ImageMessage("Hue channel isolated",cvCloneImage(grayscaleImage)));

    IplImage* binaryImage = canny(grayscaleImage, extras);

  return binaryImage;
}

CvBlobs Vision::findCandidates(IplImage *image, vector<int> skyHSV, vector<ImageMessage> &extras){
  Log::debug("Looking for images");
  assert(image->nChannels = 3 );
  assert(image->depth = IPL_DEPTH_8U);

  if (skyHSV.size() == 3){
    return CvBlobs();
  } else {
    CvBlobs blobs;
    IplImage* bwImage = fullColorToBW(image,ADAPTIVE_THRESHHOLD,extras);
    IplImage* label= cvCreateImage(cvGetSize(image),IPL_DEPTH_LABEL,1);
    unsigned int result = cvLabel(bwImage,label,blobs);
    if (intermediateSteps) {
      extras.push_back(ImageMessage("Black and White image",bwImage));
      IplImage* output = cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,3);
      cvZero(output);
      cvRenderBlobs(label,blobs,image,output);
      extras.push_back(ImageMessage("Labeled image",output));
    } else {
      cvReleaseImage(&bwImage);
    }
    cvReleaseImage(&label);
    return blobs;
  }
}

PlaneVisionMessage Vision::findPlane(IplImage* image, vector<PlaneVisionMessage> previousPlanes, vector<int> skyHSV, vector<int> planeHSV){
  static int frame = 0;
  frame++;
  if(image == NULL){
    return PlaneVisionMessage();
  }

  vector<ImageMessage> extras;

  bool useVelocity = USE_VELOCITY && previousPlanes.size() > 1;
  bool usePosition = USE_POSITION && previousPlanes.size() > 0;
  bool usePlaneSize = USE_PLANE_SIZE && previousPlanes.size() > 0; 
  bool useSkyColor = USE_SKY_COLOR && skyHSV.size() == 3;
  bool usePlaneColor = USE_PLANE_COLOR && planeHSV.size() == 3;

  Log::debug("Frame: " + frame);
  CvBlobs candidates = findCandidates(image,skyHSV,extras);
  Log::debug("Found candidates");
  if (Log::debugMode){
    for (CvBlobs::const_iterator it=candidates.begin(); it!=candidates.end(); ++it){
      cout << "Blob #" << it->second->label << ": Area=" << it->second->area;
      cout << ", Centroid=(" << it->second->centroid.x << ", " << it->second->centroid.y << ")\n";
    }
  }
  
  if (candidates.size() > 0){
    return PlaneVisionMessage(*candidates.begin()->second,image,extras);
  } else {
    return PlaneVisionMessage(image,extras);
  }
}
