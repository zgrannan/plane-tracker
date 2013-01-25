#ifndef VISION_H
#define VISION_H 

#include <vector>
#include <list>
#include <cv.h>
#include <cvblob.h>

#include "src/util/Messages.h"
#include "src/perhaps/perhaps.h"

using namespace std;
using namespace cvb;
using namespace Messages;

class Vision {
  public:
    Vision(bool intermediateSteps): intermediateSteps(intermediateSteps) {}
    /**
     * This function attempts to find the plane from the image, and uses CV along with some
     * heurestics to determine the plane's location.
     */
    PlaneVisionMessage findPlane(IplImage* image, list<PlaneVisionMessage> previousPlanes);

    void setEdgeThresholding(int thresholding){ this->edgeThresholding = thresholding; }
    void setMinBlobSize(int blobSize){ this->minBlobSize = blobSize; }
    void setMaxBlobSize(int blobSize){ this->maxBlobSize = blobSize; }
    void setUseSize(bool useSize) { this->useSize = useSize; }
    void setUsePosition(bool usePosition) { this->usePosition = usePosition; }
    void setUseRatio(bool useRatio) { this->useRatio= useRatio; }
    void setUseColor(bool useColor) { this->useColor = useColor; this->hasColor = false; }

    void setPlaneBlob(double blobX, double blobY){ 
      this->blobXOption = Some<double>(blobX);
      this->blobYOption = Some<double>(blobY);
    }

    void setPositionThresh(int thresh) {positionThresh = (double)thresh / 100.0;};
    void setSizeThresh(int thresh) {sizeThresh = (double)thresh / 100.0;}
    void setColorThresh(int thresh) {colorThresh = (double)thresh / 100.0;}

    static void testRgbToHsv();

  private:
    PlaneVisionMessage findPlane( IplImage* image, double blobX, double blobY);
    static void rgbToHsv(uchar _r, uchar _g, uchar _b, double& h, double& s, double& v);

    Option<double> blobXOption, blobYOption;

    static double f_cbrt(double r);

    double goodH, goodS, goodV;
    bool intermediateSteps;
    bool usePosition = true;
    bool useSize = true;
    bool useRatio = true;
    bool useColor = false;
    bool hasColor = false;

    int edgeThresholding = 50;
    int minBlobSize = 0;
    int maxBlobSize = 40;

    double positionThresh = 0.5;
    double sizeThresh = 0.5;
    double colorThresh = 0.5;

    /**
     * Returns all of the blobs that are returned from a CV analysis of 
     * the image, taking only the sky color into account. Later filtering will 
     * be performed on this data to reduce the number of blobs found
     */
    pair<CvBlobs,IplImage*> findCandidates(IplImage *image, vector<ImageMessage> &extras);

    /**
     * Converts the image into a binary image that is suitable for blob detection, using 
     * a given conversion method. This does most of the CV work.
     */
    IplImage* fullColorToBW (IplImage* image, vector<ImageMessage> &extras);


    /**
     * This determines the displacement between the centroids of the blobs. This is used for 
     * velocity calculation
     */
    vector<double> getDisplacement(CvBlob* currentBlob, CvBlob* lastBlob);

    IplImage* canny(IplImage* grayImage, vector<ImageMessage> &extras);

    double computeRatio(CvBlob* blob);
};
#endif
