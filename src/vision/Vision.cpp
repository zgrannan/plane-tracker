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
  IplImage* result = cvCreateImage(cvGetSize(grayImage),IPL_DEPTH_8U,1);
  IplImage* destIpl= new IplImage(dest);
  cvThreshold(destIpl,result,1,255,CV_THRESH_BINARY);
  if (intermediateSteps){
    IplImage* toDisplay = cvCreateImage(cvGetSize(grayImage),IPL_DEPTH_8U,3);
    cvCvtColor(result,toDisplay,CV_GRAY2RGB);
    extras.push_back(ImageMessage("Edge detection", toDisplay));
  }

  return result;
}

/**
 * Converts a full-color image to a black and white image that can be used
 * for blob detection.
 */
IplImage* Vision::fullColorToBW (IplImage* image,  vector<ImageMessage> &extras){
  assert(image != nullptr);
  IplImage* grayscaleImage = cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,1);
  cvCvtColor(image,grayscaleImage,CV_BGR2GRAY);
  if (intermediateSteps){
    IplImage* toDisplay = cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,3);
    cvCvtColor(grayscaleImage,toDisplay,CV_GRAY2RGB);
    extras.push_back(ImageMessage("Converted to grayscale",toDisplay));
  }

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
    IplImage* output = cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,3);
    cvZero(output);
    cvRenderBlobs(label,blobs,image,output);
    extras.push_back(ImageMessage("Labeled image",output));
  } 
  cvReleaseImage(&bwImage);
  cvReleaseImage(&label);
  return blobs;
}

double Vision::computeRatio(CvBlob* blob){
  double dx = blob->maxx - blob->minx;
  double dy = blob->maxy - blob->miny;
  return dy / dx;
}

PlaneVisionMessage Vision::findPlane( IplImage* image,
    list<PlaneVisionMessage> previousPlanes){
  static int frame = 0;
  frame++;
  if(image == nullptr){
    return PlaneVisionMessage();
  }

  vector<ImageMessage> extras;
  Log::debug("Frame: " + boost::lexical_cast<string>(frame));
  CvBlobs candidates = findCandidates(image,extras);
  int minBlobPX = ((double)minBlobSize / 20000.0) * (double)(image->height * image->width);
  int maxBlobPX = ((double)maxBlobSize / 10000.0) * (double)(image->height * image->width);
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
        double oldRatio = computeRatio(lastBlob);
        double ratio = computeRatio(blob);
        dRatio = fabs(ratio/oldRatio);
        score += BlobScore(dRatio,dPosition,dSize).computeScore();
      }
      if ( score > maxScore ){
        Log::debug("MaxScore: " + boost::lexical_cast<string>(score));
        maxScore = score;
        bestCandidate = Some<CvBlob>(*blob);
      }
      delete blob;
    }
  } else {
    if ( candidates.size() > 0 ){
      bestCandidate = Some<CvBlob>(*candidates.begin()->second);
      for (CvBlobs::const_iterator it=candidates.begin(); it!=candidates.end(); ++it){
        auto blob = it->second;
        delete blob;
      }
    } 
  }

  if (bestCandidate.isDefined()) {
    return PlaneVisionMessage(*bestCandidate,image,extras);
  } else {
    return PlaneVisionMessage(image,extras);
  }
}

double Vision::BlobScore::computeScore(){
  function<double(double)>_fabs = (double(*)(double))fabs;
  double positionWeight = 1.0;
  double sizeWeight = 0.0; 
  double ratioWeight = 0.0;
  double positionScore = -(pow(dPosition,2));
  double ratioScore = -(pow(dRatio-1,2)) * 1000.0;
  double sizeScore = -dSize / 50000.0;
  Log::debug("Position Difference: " + boost::lexical_cast<string>(dPosition));
  Log::debug("Position Score: " + boost::lexical_cast<string>(positionScore));
  Log::debug("Weighted Position Score: "  + boost::lexical_cast<string>(positionScore * positionWeight));
  Log::debug("Ratio Difference: " + boost::lexical_cast<string>(dRatio));
  Log::debug("Ratio Score: " + boost::lexical_cast<string>(ratioScore));
  Log::debug("Weighted Ratio Score: "  + boost::lexical_cast<string>(ratioScore * ratioWeight));
  Log::debug("Size Difference: " + boost::lexical_cast<string>(dRatio));
  Log::debug("Size Score: " + boost::lexical_cast<string>(sizeScore));
  Log::debug("Weighted Size Score: "  + boost::lexical_cast<string>(sizeScore * sizeWeight));
  return positionScore * positionWeight + ratioScore * ratioWeight + sizeScore * sizeWeight;
}  
