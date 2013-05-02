#include <cvblob.h>
#include <cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include "src/vision/Vision.h"
#include "src/util/Log.h"
#include "src/perhaps/perhaps.h"
//#include "src/perhaps/fun.cpp"
#include <boost/lexical_cast.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"

using namespace cvb;
using namespace std;
using namespace cv;
using namespace boost::posix_time;

void Vision::testRgbToHsv(){
  uchar r = 34, g = 52, b = 29;
  double h,s,v;
  rgbToHsv(r,g,b,h,s,v);
  cout << "HSV: " << h << " " << s << " " << v << endl;
}

/**
 * Code stolen from: 
 * http://en.literateprograms.org/RGB_to_HSV_color_space_conversion_%28C%29
 */
void Vision::rgbToHsv(uchar _r, uchar _g, uchar _b, double& h, double& s, double& v){

  double r = (double)_r / 255.0;
  double g = (double)_g / 255.0;
  double b = (double)_b / 255.0;
  auto min3 =[](const double a, const double b, const double c){
    if ( a < b && a < c) {
      return a;
    } else if ( b < a && b < c) {
      return b;
    } else {
      return c;
    }
  };

  auto max3 =[](const double a, const double b, const double c){
    if ( a > b && a > c) {
      return a;
    } else if ( b > a && b > c) {
      return b;
    } else {
      return c;
    }
  };

  double rgb_min, rgb_max;
  rgb_min = min3(r,g,b);
  rgb_max = max3(r,g,b);
  v = rgb_max;
  if (v == 0) {
    h = s = 0;
    return;
  }
  /* Normalize value to 1 */
  r /= v;
  g /= v;
  b /= v;
  rgb_min = min3(r, g, b);
  rgb_max = max3(r, g, b);
  s = rgb_max - rgb_min;
  if (s == 0) {
    h = 0;
    return;
  }
  /* Normalize saturation to 1 */
  r = (r - rgb_min)/(rgb_max - rgb_min);
  g = (g - rgb_min)/(rgb_max - rgb_min);
  b = (b - rgb_min)/(rgb_max - rgb_min);
  rgb_min = min3(r, g, b);
  rgb_max = max3(r, g, b);
  /* Compute hue */
  if (rgb_max == r) {
    h = 0.0 + 60.0*(g - b);
    if (h < 0.0) {
      h += 360.0;
    }
  } else if (rgb_max == g) {
    h = 120.0 + 60.0*(b - r);
  } else /* rgb_max == b */ {
    h = 240.0 + 60.0*(r - g);
  }
}

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

pair<CvBlobs,IplImage*> Vision::findCandidates(IplImage *image, vector<ImageMessage> &extras){
  assert(image->nChannels == 3 );
  assert(image->depth == IPL_DEPTH_8U);

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
  return {blobs,label};
}

double Vision::computeRatio(CvBlob* blob){
  double dx = blob->maxx - blob->minx;
  double dy = blob->maxy - blob->miny;
  return dy / dx;
}

PlaneVisionMessage Vision::findPlane(IplImage* image, double blobX, double blobY){
  if(image == nullptr){
    return PlaneVisionMessage();
  }
  vector<ImageMessage> extras;
  pair<CvBlobs,IplImage*> candidatesWithLabel = findCandidates(image,extras);
  auto candidates = candidatesWithLabel.first;
  Option<CvBlob> bestCandidate = None<CvBlob>();
  double bestDistance = DBL_MAX;
    for (CvBlobs::const_iterator it=candidates.begin(); it!=candidates.end(); ++it){
      auto blob = it->second;
      double dx = blob->centroid.x - blobX;
      double dy = blob->centroid.y - blobY;
      if (dx * dx + dy * dy  < bestDistance){
        bestDistance = dx * dx + dy * dy;
        bestCandidate = Some<CvBlob>(*blob);
      }
    }
    cvReleaseBlobs(candidates);
    if (bestCandidate.isDefined()) {
      return PlaneVisionMessage(*bestCandidate,image,extras,true);
    } else {
      return PlaneVisionMessage(image,extras);
    }
}

PlaneVisionMessage Vision::findPlane( IplImage* image,
    list<PlaneVisionMessage> previousPlanes){

  auto start = microsec_clock::local_time();

  if(image == nullptr){
    return PlaneVisionMessage();
  }

  if (blobXOption.isDefined() && blobYOption.isDefined()){
    PlaneVisionMessage result = findPlane(image,*blobXOption,*blobYOption);
    blobXOption = None<double>();
    blobYOption = None<double>();
    auto end = microsec_clock::local_time();
    DEBUG("Analyzed frame in: " + boost::lexical_cast<string>((end-start).total_milliseconds()) + " milliseconds");
    return result;
  }

  vector<ImageMessage> extras;
  pair<CvBlobs,IplImage*> candidatesWithLabel = findCandidates(image,extras);

  auto candidates = candidatesWithLabel.first;
  auto label = candidatesWithLabel.second;
  int minBlobPX = minBlobSize * (double)image->height;
  int maxBlobPX = maxBlobSize * (double)image->height;

  double maxScore = -DBL_MAX;
  Option<CvBlob> bestCandidate = None<CvBlob>();

  DEBUG("PROFILE: Scoring begins for " + boost::lexical_cast<string>(candidates.size())
             + " candidates, " + boost::lexical_cast<string>(previousPlanes.size()) +
             " previous planes." );

  if ( previousPlanes.size() >= 1){
    auto lastBlob = previousPlanes.front().planeBlob;
    for (CvBlobs::const_iterator it=candidates.begin(); it!=candidates.end(); ++it){
      auto blob = it->second;
      auto displacementVector = getDisplacement(blob,&lastBlob);
      double dPosition = sqrt(pow(displacementVector[0],2)+ pow(displacementVector[1],2));

      double blobHeight = blob->maxy - blob->miny;
      double blobWidth = blob->maxx - blob->minx;

      if ( blobHeight > maxBlobPX || blobHeight < minBlobPX ){
        DEBUG("Filtered blob by height: Exceeds " + boost::lexical_cast<string>(maxBlobPX));
        continue;
      }

      if ( blobWidth > maxBlobPX || blobWidth < minBlobPX ){
        DEBUG("Filtered blob by width");
        continue;
      }

      if ( dPosition > image->width * image->height / (25.0 * positionThresh)){
        DEBUG("Filtered blob by position");
        continue;
      }

      if ( sizeThresh != 0){ 
        if ( (double)blob->area / (double)lastBlob.area < (0.1 * sizeThresh) || 
             (double)blob->area / (double)lastBlob.area > (10.0 / sizeThresh)) {
          DEBUG("Filtered blob by size");
           continue;
        }
      }

      if (useColor && hasColor){
        CvScalar color = cvBlobMeanColor(blob,label,image);
        double h,s,v;
        Vision::rgbToHsv(color.val[0],color.val[1],color.val[2],h,s,v);
        double dColor = fabs(h - goodH); 
        if (dColor > 360.0 * (1.0 - colorThresh)) {
          DEBUG("colorHue: " + boost::lexical_cast<string>(h) + " goodH" +
                boost::lexical_cast<string>(goodH));
          DEBUG("Filtered blob by color: " + boost::lexical_cast<string>(dColor));
          DEBUG("colorThresh: " + boost::lexical_cast<string>(colorThresh))
          continue;
        }
      } else if (!useColor) {
        cvReleaseImage(&label);
      }
      double score = -dPosition;
      if ( score > maxScore ){
        maxScore = score;
        bestCandidate = Some<CvBlob>(*blob);
      }
    }
  } 
  cvReleaseBlobs(candidates);
  if (bestCandidate.isDefined()) {
    if (!hasColor && useColor){
      auto candidate = bestCandidate.get();
      CvScalar color = cvBlobMeanColor(&candidate,label,image);
      rgbToHsv(color.val[2],color.val[1],color.val[0],goodH,goodS,goodV);
      Log::log("Color found");
      Log::log("GoodH: " + boost::lexical_cast<string>(goodH));
      Log::log("GoodS: " + boost::lexical_cast<string>(goodS));
      Log::log("GoodV: " + boost::lexical_cast<string>(goodV));
      hasColor = true;
    }
    cvReleaseImage(&label);
    auto end = microsec_clock::local_time();
    DEBUG("Analyzed frame in: " + boost::lexical_cast<string>((end-start).total_milliseconds()) + " milliseconds");
    return PlaneVisionMessage(*bestCandidate,image,extras);
  } else {
    auto end = microsec_clock::local_time();
    DEBUG("Analyzed frame in: " + boost::lexical_cast<string>((end-start).total_milliseconds()) + " milliseconds");
    return PlaneVisionMessage(image,extras);
  }
}
