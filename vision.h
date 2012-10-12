#ifndef VISION_H
#define VISION_H 

/**
 * This class represents the result of a successful CV plane detection
 * planeBlob: The CvBlob that the plane has been identified as
 * image: The image that captured by the camera
 */
class PlaneData {
public:
	CvBlob* planeBlob;
	IplImage* image;
	PlaneData(CvBlob* planeBlob, IplImage* image);
};

class BlobScore {
public: 
	float ratioScore; 		// The score given based on the blobs width/height ratio
	float velocityScore; 	// Score based on current velocity vs expected velocity
	float positionScore; 	// Score based on current position vs expected position
	float colorScore; 		// Plane color vs Expected color
	float sizeScore; 		// Plane size vs Expected size
	float gpsScore; 		// Visual plane position vs GPS plane position
	float computeScore();	// Computes a weighted some of the scores to determine a confidence
							// level for this blob
}

/**
 * Returns all of the blobs that are returned from a CV analysis of 
 * the image, taking only the sky color into account. Later filtering will 
 * be performed on this data to reduce the number of blobs found
 */
CvBlobs* findCandidates(IplImage *image, vector<int>* skyHSV){

/**
 * Converts the image into a binary image that is suitable for blob detection, using 
 * a given conversion method. This does most of the CV work.
 */
IplImage* fullColorToBW (IplImage* image, int conversionMethod){

/**
 * This function attempts to find the plane from the image, and uses CV along with some
 * heurestics to determine the plane's location.
 */
PlaneData* findPlane(IplImage* image, vector<PlaneData*>* previousPlanes, vector<int>* skyHSV, vector<int>* planeHSV){

/**
 * This determines the displacement between the centroids of the blobs. This is used for 
 * velocity calculation
 */
vector<double>* getDisplacement(CvBlob* currentBlob, CvBlob* lastBlob) {

/**
 * This is a test utility that displays an image on the screen at a given
 * scale.
 * name: The window name
 * scale: The scale rate (1.0 = normal resolution)
 */
void showImage(string name, IplImage* image, float scale){

#endif
