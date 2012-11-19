#include <cvblob.h>
#include <cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include "src/vision/Vision.h"
#include "src/util/Log.h"
#include "src/perhaps/perhaps.h"
#include "src/perhaps/fun.cpp"
#include <boost/lexical_cast.hpp>

/* Plane detection flags */
#define USE_POSITION true
#define USE_PLANE_SIZE true
#define USE_RATIO true

/* Color -> Black and White conversion technique */
#define ADAPTIVE_THRESHHOLD 0

using namespace cvb;
using namespace std;
using namespace cv;

vector<double> Vision::getDisplacement(CvBlob* blob1, CvBlob* blob2){
  double dx = blob2->centroid.x - blob1->centroid.x;
  double dy = blob2->centroid.y - blob1->centroid.y;
  return {dx,dy};
}

IplImage* Vision::canny(IplImage* grayImage, vector<ImageMessage> &extras){
  int ratio = 3;
  int kernelSize =3;
  Mat edges, dest;
  blur(Mat(grayImage), edges, Size(3,3));
  Log::debug("Edge thresholding is: "+ boost::lexical_cast<string>(edgeThresholding));
  Canny(edges, edges, edgeThresholding ,edgeThresholding * ratio, kernelSize);
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
IplImage* Vision::fullColorToBW (IplImage* image,  vector<ImageMessage> &extras){
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
  cvReleaseImage(&grayscaleImage);
  return binaryImage;
}

CvBlobs Vision::findCandidates(IplImage *image, vector<ImageMessage> &extras){
  Log::debug("Looking for images");
  assert(image->nChannels = 3 );
  assert(image->depth = IPL_DEPTH_8U);

  CvBlobs blobs;
  IplImage* bwImage = fullColorToBW(image,extras);
  IplImage* label= cvCreateImage(cvGetSize(image),IPL_DEPTH_LABEL,1);
  cvLabel(bwImage,label,blobs);
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

PlaneVisionMessage Vision::findPlane( IplImage* image,
    list<PlaneVisionMessage> previousPlanes){
  static int frame = 0;
  frame++;
  if(image == NULL){
    return PlaneVisionMessage();
  }

  vector<ImageMessage> extras;
  Log::debug("Frame: " + boost::lexical_cast<string>(frame));
  CvBlobs candidates = findCandidates(image,extras);

  int minBlobPX = ( pow(minBlobSize,2.0) / 10000.0 ) * (image->height * image->width);
  int maxBlobPX = ( pow(maxBlobSize,2.0) / 10000.0 ) * (image->height * image->width);
  cvFilterByArea(candidates,minBlobPX,maxBlobPX);
  double maxScore = -DBL_MAX;
  Option<CvBlob> bestCandidate = None<CvBlob>();
  Log::debug("Found candidates");
  if ( previousPlanes.size() >= 1){
    for (CvBlobs::const_iterator it=candidates.begin(); it!=candidates.end(); ++it){
      double score = 0;
      auto blob = it->second;
      for ( auto lastPlane : previousPlanes) {
        auto lastBlob = &lastPlane.planeBlob; 
        double dPosition = 0, dRatio = 0, dSize = 0;
        auto displacementVector = getDisplacement(blob,lastBlob);
        dPosition = sqrt(pow(displacementVector[0],2)+ pow(displacementVector[1],2));
        dSize = fabs(blob->area-lastBlob->area);
        double oldRatio = lastBlob->centroid.y / lastBlob->centroid.x;
        double ratio = blob->centroid.y / blob->centroid.x;
        dRatio = fabs(ratio-oldRatio);
        score += BlobScore(dRatio,dPosition,dSize).computeScore();
      }
      if ( score > maxScore ){
        Log::debug("MaxScore: " + boost::lexical_cast<string>(score));
        maxScore = score;
        bestCandidate = Some<CvBlob>(*blob);
      }
    }
  } else {
    if ( candidates.size() > 0 ){
      bestCandidate = Some<CvBlob>(*candidates.begin()->second);
    } 
  }
  // cout << "Blob #" << it->second->label << ": Area=" << it->second->area;
  // cout << ", Centroid=(" << it->second->centroid.x << ", " << it->second->centroid.y << ")\n";

  if (bestCandidate.isDefined()) {
    return PlaneVisionMessage(*bestCandidate,image,extras);
  } else {
    Log::debug("NPF");
    return PlaneVisionMessage(image,extras);
  }
}

double Vision::BlobScore::computeScore(){
  function<double(double)>_fabs = (double(*)(double))fabs;
  double positionScore = -(pow(dPosition,2));
  double ratioScore = -(pow(dRatio,2));
  double sizeScore = -(pow(dSize,2));
  return positionScore * positionWeight + ratioScore * ratioWeight + sizeScore * sizeWeight;
}  
