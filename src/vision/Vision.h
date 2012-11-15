#ifndef VISION_H
#define VISION_H 

#include <vector>
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
    PlaneVisionMessage findPlane(IplImage* image, vector<PlaneVisionMessage> previousPlanes);

    void setEdgeThresholding(int thresholding){ this->edgeThresholding = thresholding; }
    void setMinBlobSize(int blobSize){ this->minBlobSize = blobSize; }
    void setMaxBlobSize(int blobSize){ this->maxBlobSize = blobSize; }
    void setUseSize(bool useSize) { this->useSize = useSize; }
    void setUsePosition(bool usePosition) { this->usePosition= usePosition; }
    void setUseRatio(bool useRatio) { this->useRatio= useRatio; }

  private:
    bool intermediateSteps;

    bool usePosition = true;
    bool useSize = true;
    bool useRatio = true;

    int edgeThresholding = 50;
    int minBlobSize = 5;
    int maxBlobSize = 40;

    class BlobScore {
      public: 
        float ratioScore; 		// The score given based on the blobs width/height ratio
        float positionScore; 	// Score based on current position vs expected position
        float sizeScore; 		// Plane size vs Expected size
        float computeScore();	// Computes a weighted some of the scores to
                                // determine a confidence level for this blob
    };

    /**
     * Returns all of the blobs that are returned from a CV analysis of 
     * the image, taking only the sky color into account. Later filtering will 
     * be performed on this data to reduce the number of blobs found
     */
    CvBlobs findCandidates(IplImage *image, vector<ImageMessage> &extras);

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
};
#endif
