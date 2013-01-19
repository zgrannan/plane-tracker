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


double Vision::f_cbrt(double r){
	r/=65535.0;
	return r>ep?pow(r,1/3.0):(ka*r+16)/116.0;
}

void Vision::rgbToCielab(uchar _r, uchar _g, uchar _b, double& l, double& a, double& b){
  static double d50_white[3]={0.964220,1,0.825211};

  double xyz[3] = {0,0,0};
  double red = (double) _r / 255.0;
  double green = (double) _g / 255.0;
  double blue = (double) _b / 255.0;

  static const double rgb_xyz[3][3] =
	{ { 0.7976748465, 0.1351917082, 0.0313534088 },
	  { 0.2880402025, 0.7118741325, 0.0000856651 },
	  { 0.0000000000, 0.0000000000, 0.8252114389 } }; // From Jacques Desmis

  for (int i = 0; i < 3; i++){
    xyz[i] += rgb_xyz[i][0]*red;
    xyz[i] += rgb_xyz[i][1]*green;
    xyz[i] += rgb_xyz[i][2]*blue;
  }

  for (int i = 0; i < 3; i++){
    xyz[i] = f_cbrt(xyz[i]/d50_white[i]);
  }

  l = 116.0 * xyz[1]-16.0;
  a = 500.0 * (xyz[0]-xyz[1]);
  b = 200.0 * (xyz[1]-xyz[2]);

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
  DEBUG("Edge thresholding is: "+ boost::lexical_cast<string>(edgeThresholding));
  Canny(edges, edges, edgeThresholding ,edgeThresholding * ratio, kernelSize);
  dest = Scalar::all(0);
  Mat(grayImage).copyTo(dest, edges);
  IplImage* result = cvCreateImage(cvGetSize(grayImage),IPL_DEPTH_8U,1);
  IplImage* destIpl= new IplImage(dest);
  cvThreshold(destIpl,result,1,255,CV_THRESH_BINARY);
  if (false){
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
  if (false){
    IplImage* toDisplay = cvCreateImage(cvGetSize(image),IPL_DEPTH_8U,3);
    cvCvtColor(grayscaleImage,toDisplay,CV_GRAY2RGB);
    extras.push_back(ImageMessage("Converted to grayscale",toDisplay));
  }

  IplImage* binaryImage = canny(grayscaleImage, extras);
  cvReleaseImage(&grayscaleImage);
  return binaryImage;
}

pair<CvBlobs,IplImage*> Vision::findCandidates(IplImage *image, vector<ImageMessage> &extras){
  DEBUG("Looking for images");
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
  return {blobs,label};
}

double Vision::computeRatio(CvBlob* blob){
  double dx = blob->maxx - blob->minx;
  double dy = blob->maxy - blob->miny;
  return dy / dx;
}

PlaneVisionMessage Vision::findPlane( IplImage* image, double blobX, double blobY){
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
  DEBUG("PROFILE: findPlane begins");
  static int frame = 0;
  frame++;
  if(image == nullptr){
    return PlaneVisionMessage();
  }

  if (blobXOption.isDefined() && blobYOption.isDefined()){
    PlaneVisionMessage result = findPlane(image,*blobXOption,*blobYOption);
    blobXOption = None<double>();
    blobYOption = None<double>();
    return result;
  }

  bool canUseColor = true;

  vector<ImageMessage> extras;
  DEBUG("PROFILE: findCandidates begins");
  pair<CvBlobs,IplImage*> candidatesWithLabel = findCandidates(image,extras);
  DEBUG("PROFILE: findCandidates ends");
  auto candidates = candidatesWithLabel.first;
  auto label = candidatesWithLabel.second;
  int minBlobPX = ((double)minBlobSize / 20000.0) * (double)(image->height * image->width);
  int maxBlobPX = ((double)maxBlobSize / 10000.0) * (double)(image->height * image->width);
  DEBUG("PROFILE: FilterByArea begins");
  cvFilterByArea(candidates,minBlobPX,maxBlobPX);
  DEBUG("PROFILE: FilterByArea ends");
  double maxScore = -DBL_MAX;
  Option<CvBlob> bestCandidate = None<CvBlob>();
  DEBUG("PROFILE: Scoring begins for " + boost::lexical_cast<string>(candidates.size())
             + " candidates, " + boost::lexical_cast<string>(previousPlanes.size()) +
             " previous planes." );
  if ( previousPlanes.size() >= 1){
    for (CvBlobs::const_iterator it=candidates.begin(); it!=candidates.end(); ++it){
      double score = 0;
      auto blob = it->second;
      for ( auto lastPlane : previousPlanes) {
        auto lastBlob = &lastPlane.planeBlob; 
        double dPosition = 0, dRatio = 0, dSize = 0;
        DEBUG("PROFILE: Get Displacement Start");
        auto displacementVector = getDisplacement(blob,lastBlob);
        DEBUG("PROFILE: Get Displacement End");
        DEBUG("PROFILE: Compute Position/Size Start");
        dPosition = sqrt(pow(displacementVector[0],2)+ pow(displacementVector[1],2));
        dSize = fabs(blob->area-lastBlob->area);
        DEBUG("PROFILE: Compute Position/Size End");
        DEBUG("PROFILE: Compute Ratio Start");
        double oldRatio = computeRatio(lastBlob);
        double ratio = computeRatio(blob);
        DEBUG("PROFILE: Compute Ratio End");
        double dColor = 0;
        if (useColor && hasColor){
          CvScalar color = cvBlobMeanColor(blob,label,image);
          double l,a,b;
          rgbToCielab(color.val[0],color.val[1],color.val[2],l,a,b);
          DEBUG("L: " + boost::lexical_cast<string>(l));
          DEBUG("A: " + boost::lexical_cast<string>(a));
          DEBUG("B: " + boost::lexical_cast<string>(b));
          double dL2 = pow(l - goodL,2); 
          double dA2 = pow(l - goodA,2); 
          double dB2 = pow(l - goodB,2); 
          dColor = sqrt(dL2 + dA2 + dB2);
        } else if (!useColor) {
          canUseColor = false;
          cvReleaseImage(&label);
        }
        dRatio = fabs(ratio/oldRatio);
        DEBUG("PROFILE: Compute Score Start");
        score += BlobScore(dRatio,dPosition,dSize,dColor,
                            ratioWeight,positionWeight,sizeWeight,colorWeight).computeScore();
        DEBUG("PROFILE: Compute Score End");
      }
      if ( score > maxScore ){
        DEBUG("MaxScore: " + boost::lexical_cast<string>(score));
        maxScore = score;
        bestCandidate = Some<CvBlob>(*blob);
      }
    }
  } else {
    if ( candidates.size() > 0 ){
      bestCandidate = Some<CvBlob>(*candidates.begin()->second);
    } 
  }
  DEBUG("PROFILE: Scoring ends");
  DEBUG("PROFILE: cvReleaseBlobs begins");
  cvReleaseBlobs(candidates);
  DEBUG("PROFILE: cvReleaseBlobs ends");
  DEBUG("PROFILE: findPlane ends");
  if (bestCandidate.isDefined()) {
    if (!hasColor && useColor && canUseColor){
      auto candidate = bestCandidate.get();
      CvScalar color = cvBlobMeanColor(&candidate,label,image);
      rgbToCielab(color.val[0],color.val[1],color.val[2],goodL,goodA,goodB);
      Log::log("Color found");
      Log::log("GoodL: " + boost::lexical_cast<string>(goodL));
      Log::log("GoodA: " + boost::lexical_cast<string>(goodA));
      Log::log("GoodB: " + boost::lexical_cast<string>(goodB));
      hasColor = true;
    }
    cvReleaseImage(&label);
    return PlaneVisionMessage(*bestCandidate,image,extras);
  } else {
    return PlaneVisionMessage(image,extras);
  }
}

double Vision::BlobScore::getScore(double diff, double& maxDiff){
  if ( diff > maxDiff ){
    maxDiff = diff;
    return 0;
  } else {
    if ( maxDiff == 0 ) return 0;
    return 1 - (diff / maxDiff);
  }
}

double Vision::BlobScore::computeScore(){
  static double maxdRatio = 0;
  static double maxdPosition = 0;
  static double maxdSize = 0;
  static double maxdColor = 0;
  
  double positionScore = getScore(dPosition,maxdPosition);
  double ratioScore = getScore(dRatio,maxdRatio);
  double sizeScore = getScore(dSize,maxdSize);
  double colorScore = getScore(dColor,maxdColor);
  /**
  DEBUG("Position Difference: " + boost::lexical_cast<string>(dPosition));
  DEBUG("Position Score: " + boost::lexical_cast<string>(positionScore));
  DEBUG("Weighted Position Score: "  + boost::lexical_cast<string>(positionScore * positionWeight));
  DEBUG("Ratio Difference: " + boost::lexical_cast<string>(dRatio));
  DEBUG("Ratio Score: " + boost::lexical_cast<string>(ratioScore));
  DEBUG("Weighted Ratio Score: "  + boost::lexical_cast<string>(ratioScore * ratioWeight));
  DEBUG("Size Difference: " + boost::lexical_cast<string>(dColor));
  DEBUG("Size Score: " + boost::lexical_cast<string>(sizeScore));
  DEBUG("Weighted Size Score: "  + boost::lexical_cast<string>(sizeScore * sizeWeight));
  DEBUG("Color Difference: " + boost::lexical_cast<string>(dColor));
  DEBUG("Color Score: " + boost::lexical_cast<string>(colorScore));
  DEBUG("Weighted Color Score: "  + boost::lexical_cast<string>(colorScore * colorWeight));
  **/
  return positionScore * positionWeight +
         ratioScore * ratioWeight +
         sizeScore * sizeWeight + 
         colorScore * colorWeight;
}  
