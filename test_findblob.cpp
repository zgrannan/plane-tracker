#include <cvblob.h>
#include <cv.h>
#include <highgui.h>
using namespace cvb;
using namespace std;
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

