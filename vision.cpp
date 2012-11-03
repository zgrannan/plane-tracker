#include <cvblob.h>
#include <cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include "Vision.h"

using namespace Vision;

/* Plane detection flags */
#define USE_VELOCITY true
#define USE_POSITION true
#define USE_PLANE_SIZE true
#define USE_SKY_COLOR true
#define USE_PLANE_COLOR true

/* Color -> Black and White conversion technique */
#define ADAPTIVE_THRESHHOLD 0

#define VISION_DEBUG true

using namespace cvb;
using namespace std;
using namespace cv;

/**
 * Displays the image on the screen for debugging purposes
 */
IplImage* Vision::showImage(string name, IplImage* image, float scale){
  CvSize newSize = cvSize((int)(image->width * scale),(int)(image->height * scale));
  IplImage* newImage = cvCreateImage(newSize,image->depth,image->nChannels);
  cvResize(image,newImage); 
  imshow(name,Mat(newImage));
  cvReleaseImage(&newImage);
  return newImage;
}


vector<double> Vision::getVelocityVector(CvBlob currentBlob, CvBlob lastBlob) {
  double dx = currentBlob.centroid.x - lastBlob.centroid.x;
  double dy = currentBlob.centroid.y - lastBlob.centroid.y;
  vector<double> result;
  result.push_back(dx);
  result.push_back(dy);
  return result;
}

/**
 * Converts a full-color image to a black and white image that can be used
 * for blob detection.
 */
IplImage* Vision::fullColorToBW (IplImage* image, int conversionMethod, vector<ImageMessage> &extras){
  IplImage* binaryImage = cvCreateImage(cvGetSize(image),8,1);

  /**
   * Uses adaptive threshholding to yield a black and white image. This gets the best results
   * so far
   */
  if (conversionMethod == ADAPTIVE_THRESHHOLD){
    IplImage *hsvImage = cvCreateImage(cvGetSize(image),image->depth,image->nChannels);

    // Create a hue-saturation-value version of this image
    cvCvtColor(image,hsvImage,CV_BGR2HSV);

    // Isolate the hue channel
    cvSplit(hsvImage,binaryImage,NULL,NULL,NULL);

    cvReleaseImage(&hsvImage);
    extras.push_back(ImageMessage("Hue channel isolated",cvCloneImage(binaryImage)));

    // Apply adaptive threshholding
    cvAdaptiveThreshold(binaryImage,binaryImage,255,ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY,41,20);
    extras.push_back(ImageMessage("Thresholding applied", cvCloneImage(binaryImage)));

    // Erode the image to increase the saliency of the plane	
    cvErode(binaryImage,binaryImage,0,4);
    extras.push_back(ImageMessage("Erosion applied", cvCloneImage(binaryImage)));

    // Invert the colors for blob detection to work
    cvNot(binaryImage,binaryImage);

  }
  return binaryImage;
}

CvBlobs Vision::findCandidates(IplImage *image, vector<int> skyHSV, vector<ImageMessage> &extras){
  cout <<"Looking for images\n";
  assert(image != NULL);

  if (skyHSV.size() == 3){
    return CvBlobs();
  } else {
    IplImage* bwImage = fullColorToBW(image,ADAPTIVE_THRESHHOLD,extras);
    IplImage* label= cvCreateImage(cvGetSize(image),IPL_DEPTH_LABEL,1);
    IplImage* output = cvCreateImage(cvGetSize(image),image->depth,image->nChannels);
    CvBlobs blobs;
    unsigned int result = cvLabel(bwImage,label,blobs);
    extras.push_back(ImageMessage("Black and White image",bwImage));
    cvRenderBlobs(label,blobs,image,output);
    extras.push_back(ImageMessage("Labeled image",output));
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

  cerr << "\nFrame: " << frame << endl;
  CvBlobs candidates = findCandidates(image,skyHSV,extras);

  cerr <<"Found candidates\n";
  for (CvBlobs::const_iterator it=candidates.begin(); it!=candidates.end(); ++it){
    cout << "Blob #" << it->second->label << ": Area=" << it->second->area;
    cout << ", Centroid=(" << it->second->centroid.x << ", " << it->second->centroid.y << ")\n";
  }
  cerr << "Done iterating\n";

  if (candidates.size() > 0){
    return PlaneVisionMessage(*candidates.begin()->second,image,extras);
  } else {
    return PlaneVisionMessage(image);
  }
}
