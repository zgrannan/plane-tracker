#ifndef VISION_H
#define VISION_H 

#include <vector>
#include <list>
#include <cv.h>
#include <cvblob.h>

#include "src/util/Messages.h"

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
    void setUsePosition(bool usePosition) { this->usePosition= usePosition; }
    void setUseRatio(bool useRatio) { this->useRatio= useRatio; }
    void setUseColor(bool useColor) { this->useColor = useColor; this->hasColor = false; }

    void setPositionWeight(int weight) {positionWeight = (double)weight / 100.0;};
    void setRatioWeight(int weight) {ratioWeight = (double)weight / 100.0;}
    void setSizeWeight(int weight) {sizeWeight = (double)weight / 100.0;}
    void setColorWeight(int weight) {colorWeight = (double)weight / 100.0;}

  private:
    static constexpr double ep = 216.0/24389.0;
    static constexpr double ka = 24389.0/27.0;

    static double f_cbrt(double r);
    static void rgbToCielab(uchar _r, uchar _g, uchar _b,
                                    double& l, double& a, double& b);

    double goodL, goodA, goodB;

    bool intermediateSteps;
    bool usePosition = true;
    bool useSize = true;
    bool useRatio = true;
    bool useColor = false;
    bool hasColor = false;

    int edgeThresholding = 50;
    int minBlobSize = 0;
    int maxBlobSize = 40;

    double ratioWeight = 0;
    double positionWeight = 0;
    double sizeWeight = 0;
    double colorWeight = 0;

    class BlobScore {
      public: 
        BlobScore(double dRatio, double dPosition, double dSize, double dColor,
                  double ratioWeight, double positionWeight, double sizeWeight,
                  double colorWeight): dRatio(dRatio),
                                       dPosition(dPosition),
                                       dSize(dSize),
                                       dColor(dColor),
                                       ratioWeight(ratioWeight),
                                       positionWeight(positionWeight),
                                       sizeWeight(sizeWeight),
                                       colorWeight(colorWeight){}
                                                       
        BlobScore(){}
        double computeScore();	// Uses these displacements to compute the plane score   
      private:

        double getScore(double diff, double& maxdiff);



        double dRatio; 		// The change in the plane's width/height ratio
        double dPosition; 	// The change in the plane's position 
        double dSize; 		// The change in the plane's size
        double dColor;      // The change in the plane's color
        double ratioWeight;
        double positionWeight;
        double sizeWeight;
        double colorWeight;
    };

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
