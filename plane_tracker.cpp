#include <cvblob.h>
#include <cv.h>
#include <highgui.h>
using namespace cvb;
using namespace std;

struct PlaneData {
	CvBlob planeBlob;
	IplImage* planeImage;
	IplImage* labeledImage;
	float dx, dy;
	
	PlaneData(CvBlob planeBlob, IplImage* planeImage, IplImage* labeledImage) {
		this->planeBlob = planeBlob;
		this->planeImage = planeImage;
		this->labeledImage = labeledImage;
		dx = planeBlob->centroid.x - labeledImage->width / 2;
		dy = planeBlob->centroid.y - labeledImage->height / 2;
	}

}

vector<float>* getVelocityVector(vector<CvBlob>* previousBlobs){
	if (previousBlobs->size == 0) return {0,0};	
	float x = 0, y = 0;
	for each(CvBlob blob in previousBlobs){
		x += blob->centroid.x;
		y += blob->centroid.y;
	}
	return { x / vector->size, y / vector->size };
}

PlaneData findPlane(IplImage* image, vector<CvBlob>* previousBlobs, vector<int> skyRGB, vector<int> planeRGB){
	vector<float>* velocity = getVelocityVector(previousBlobs);
	
	bool useVelocity = USE_VELOCITY && previousBlobs->size > 1;
	bool usePosition = USE_POSITION && previousBlobs->size > 0;
	bool usePlaneSize = USE_PLANE_SIZE && previousBlobs->size > 0; 
	bool useSkyColor = USE_SKY_COLOR && skyRGB->size == 3;
	bool usePlaneColor = USE_PLANE_COLOR && planeRGB->size == 3;
	
	if ( !useSkyColor ) skyRGB = {};
	if ( !usePlaneColor ) colorRGB = {};
	
	CvBlobs candidates = findCandidates(image,skyRGB,planeRGB);

if ( previousBlobs->size <= 0 ) {
			
	} else {
		vector<float> 
int main(int argc, char** argv){
	IplImage *testImage = cvLoadImage("test_image.jpg");
	IplImage *greenImage = cvCreateImage(cvGetSize(testImage),8,1);
	
	// Set greenImage as a grayscale image created from the green channel of testImage 
	cvSplit(testImage,NULL,greenImage,NULL,NULL);
	
	// Convert greenImage to black and white
	cvThreshold(greenImage,greenImage,150,255,CV_THRESH_BINARY);
	
	// Create the image that will hold the labels from blob detection
	IplImage *labelImage = cvCreateImage(cvGetSize(greenImage), IPL_DEPTH_LABEL,1);

	CvBlobs blobs;

	// Find the blobs
	unsigned int result = cvLabel(greenImage,labelImage,blobs);

	// Render the blobs back onto the original image
	cvRenderBlobs(labelImage,blobs,testImage,testImage);	
	
	cvShowImage("Labeled image", testImage);
	for (CvBlobs::const_iterator it=blobs.begin(); it!=blobs.end(); ++it){
  		cout << "Blob #" << it->second->label << ": Area=" << it->second->area;
		cout << ", Centroid=(" << it->second->centroid.x << ", " << it->second->centroid.y << ")\n";
	}
	cvWaitKey(0);
	return 0;
}

