#include <cvblob.h>
#include <cv.h>
#include <highgui.h>
#include <vector>
#include "plane_tracker.h"

/* Plane detection flags */
#define USE_VELOCITY true
#define USE_POSITION true
#define USE_PLANE_SIZE true
#define USE_SKY_COLOR true
#define USE_PLANE_COLOR true

/* Color -> Black and White conversion technique */
#define THRESHHOLD 0
#define ERODE 1
#define SOBEL 2
#define ADAPTIVE_THRESHHOLD 3

/* The erosion type to use for cvErode */
#define EROSION_TYPE MORPH_RECT
// #define EROSION_TYPE MORPH_CROSS
// #define EROSION_TYPE MORPH_ELLIPSE
#define EROSION_SIZE 5

/* The default scale to display images at */
#define DEFAULT_SCALE 0.25

using namespace cvb;
using namespace std;
using namespace cv;

/**
 * Displays the image on the screen for debugging purposes
 */
void showImage(string name, IplImage* image, float scale = DEFAULT_SCALE){
	CvSize newSize = cvSize((int)(image->width * scale),(int)(image->height * scale));
	IplImage* newImage = cvCreateImage(newSize,image->depth,image->nChannels);
	cvResize(image,newImage);
	cvShowImage(name.c_str(),newImage);
	// TODO: Free the memory for newImage
}

class PlaneData {
	CvBlob* planeBlob;
	IplImage* image;
	
	PlaneData(CvBlob* planeBlob, IplImage* image) {
		this->planeBlob = planeBlob;
		this->image = image;
	}

};

vector<double>* getVelocityVector(CvBlob* currentBlob, CvBlob* lastBlob) {
	return NULL;
}

/**
 * Converts a full-color image to a black and white image that can be used
 * for blob detection.
 */
IplImage* fullColorToBW (IplImage* image, int conversionMethod){
	IplImage* binaryImage = cvCreateImage(cvGetSize(image),8,1);

	/**
	 * Very basic conversion to a black and white image. This yields
	 * very poor results
	 */
	if (conversionMethod == THRESHHOLD){
		// Convert the color image into a grayscale image
		cvCvtColor(image,binaryImage,CV_RGB2GRAY);
		// Convert the grayscale image into a black-and-white image
		cvThreshold(binaryImage,binaryImage,128,255,CV_THRESH_BINARY);
	} 
	
	/**
	 * Uses erosion to increase the saliency of the plane, and then does a threshhold
	 * conversion to BW. This yields mediocre results
	 */
	if (conversionMethod == ERODE){
		IplImage* erodedImage = cvCreateImage(cvGetSize(image),image->depth,image->nChannels);
		Mat imageMat = image, erodedMat = erodedImage;

		// Define the erosion operator
		Mat element = getStructuringElement(EROSION_TYPE,
											Size(2 * EROSION_SIZE + 1,2 * EROSION_SIZE + 1),
											Point(EROSION_SIZE,EROSION_SIZE));
		// Apply the erosion operator
		erode(imageMat,erodedMat,element);
		showImage("Eroded image",erodedImage);

		// Convert the image to black and white
		binaryImage = fullColorToBW(erodedImage,THRESHHOLD);
	} 
	/**
	 * Uses a sobel filter to get the edges from the image. In theory this should yield the 
	 * outline of the plane, making it good for blob detection. But it's not working 
	 * right now
	 */
	if (conversionMethod == SOBEL){
		// Using the sobel operation requires destination image with 16-bit depth per channel
		IplImage *deepImage= cvCreateImage(cvGetSize(image),16,1);
		// Convert to grayscale 
		cvCvtColor(image,binaryImage,CV_RGB2GRAY); //TODO: Only use the hue channel
		// Apply sobel filter
		cvSobel(binaryImage,deepImage,1,1,1);
		showImage("Sobel image",deepImage);
	} 
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
		showImage("Hue channel isolated",binaryImage);
		// Apply adaptive threshholding
		cvAdaptiveThreshold(binaryImage,binaryImage,255,ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY,41,20);
		showImage("Threshholding applied", binaryImage);
		// Erode the image to increase the saliency of the plane	
		cvErode(binaryImage,binaryImage,0,4);
		showImage("Erosion applied", binaryImage);
		// Invert the colors for blob detection to work
		cvNot(binaryImage,binaryImage);
	}
	return binaryImage;
}

CvBlobs* findCandidates(IplImage *image, vector<int>* skyHSV){
	assert(image != NULL);
	assert(skyHSV != NULL);

	if (skyHSV->size() == 3){
		return NULL;
	} else {
		IplImage* bwImage = fullColorToBW(image,ADAPTIVE_THRESHHOLD);
		IplImage* label= cvCreateImage(cvGetSize(image),IPL_DEPTH_LABEL,1);
		IplImage* output = cvCreateImage(cvGetSize(image),image->depth,image->nChannels);
		CvBlobs* blobs = new CvBlobs();
		unsigned int result = cvLabel(bwImage,label,*blobs);
		showImage("Original image", image);
		showImage("Black and White image",bwImage);
		cvRenderBlobs(label,*blobs,image,output);
		showImage("Labeled image",output);
		return blobs;
	}
}

PlaneData* findPlane(IplImage* image, vector<PlaneData*>* previousPlanes, vector<int>* skyHSV, vector<int>* planeHSV){
	
	assert(image != NULL);
	assert(previousPlanes != NULL);
	assert(skyHSV != NULL);
	assert(planeHSV != NULL);

	bool useVelocity = USE_VELOCITY && previousPlanes->size() > 1;
	bool usePosition = USE_POSITION && previousPlanes->size() > 0;
	bool usePlaneSize = USE_PLANE_SIZE && previousPlanes->size() > 0; 
	bool useSkyColor = USE_SKY_COLOR && skyHSV->size() == 3;
	bool usePlaneColor = USE_PLANE_COLOR && planeHSV->size() == 3;
	
	CvBlobs* candidates = findCandidates(image,skyHSV);
	
	for (CvBlobs::const_iterator it=candidates->begin(); it!=candidates->end(); ++it){
  		cout << "Blob #" << it->second->label << ": Area=" << it->second->area;
		cout << ", Centroid=(" << it->second->centroid.x << ", " << it->second->centroid.y << ")\n";
	}
	return NULL;
}
int main(int argc, char** argv){
	if (argc == 1){
		cout << "An image file is required.\n";
		return NULL;
	}
	
	IplImage* image = cvLoadImage(argv[1]);
	vector<PlaneData*>* previousPlanes = new vector<PlaneData*>();
	vector<int>* skyHSV= new vector<int>();
	vector<int>* planeHSV = new vector<int>();

	PlaneData* data = findPlane(image,previousPlanes,skyHSV,planeHSV);

	cvWaitKey(0);
	return 0;
}
