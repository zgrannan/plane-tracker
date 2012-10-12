#include <cvblob.h>
#include <cv.h>
#include <highgui.h>
#include <vector>

/* Plane detection flags */
#define USE_VELOCITY true
#define USE_POSITION true
#define USE_PLANE_SIZE true
#define USE_SKY_COLOR true
#define USE_PLANE_COLOR true

/* Color -> Black and White conversion technique */
#define RAW_CONVERT 0
#define ERODE 1
#define SOBEL 2
#define ADAPTIVE_THRESHHOLD 3

#define EROSION_TYPE MORPH_RECT
// #define EROSION_TYPE MORPH_CROSS
// #define EROSION_TYPE MORPH_ELLIPSE
#define EROSION_SIZE 5

#define DEFAULT_SCALE 0.25

using namespace cvb;
using namespace std;
using namespace cv;

void showImage(string name, IplImage* image, float scale = DEFAULT_SCALE){
	CvSize newSize = cvSize((int)(image->width * scale),(int)(image->height * scale));
	cerr <<"New size: ("<<newSize.width<<","<<newSize.height<<")\n";
	IplImage* newImage = cvCreateImage(newSize,image->depth,image->nChannels);
	cvResize(image,newImage);
	cvShowImage(name.c_str(),newImage);
//	delete newImage;
}
struct PlaneData {
	CvBlob* planeBlob;
	IplImage* planeImage;
	IplImage* labeledImage;
	double dx, dy;
	
	PlaneData(CvBlob* planeBlob, IplImage* planeImage, IplImage* labeledImage) {
		this->planeBlob = planeBlob;
		this->planeImage = planeImage;
		this->labeledImage = labeledImage;
		dx = planeBlob->centroid.x - labeledImage->width / 2;
		dy = planeBlob->centroid.y - labeledImage->height / 2;
	}

};

vector<double>* getVelocityVector(CvBlob* currentPlaneBlob, CvBlob* lastPlaneBlob) {
	return NULL;
	//new vector<double>(currentPlaneBlob->centroid.x - lastPlaneBlob->centroid.x,
	//		currentPlaneBlob->centroid.y - lastPlaneBlob->centroid.y);
}

IplImage* fullColorToBW (IplImage* image, int conversionMethod){
	IplImage* binaryImage = cvCreateImage(cvGetSize(image),8,1);
	if (conversionMethod == RAW_CONVERT){
		cvCvtColor(image,binaryImage,CV_RGB2GRAY);
		cvThreshold(binaryImage,binaryImage,128,255,CV_THRESH_BINARY);
	} else if (conversionMethod == ERODE){
		IplImage* erodedImage = cvCreateImage(cvGetSize(image),image->depth,image->nChannels);
		Mat imageMat = image, erodedMat = erodedImage;
		Mat element = getStructuringElement(EROSION_TYPE,
											Size(2 * EROSION_SIZE + 1,2 * EROSION_SIZE + 1),
											Point(EROSION_SIZE,EROSION_SIZE));
		erode(imageMat,erodedMat,element);
		showImage("Eroded image",erodedImage);
		binaryImage = fullColorToBW(erodedImage,RAW_CONVERT);
	} else if (conversionMethod == SOBEL){
		IplImage *deepImage= cvCreateImage(cvGetSize(image),16,1);
		Mat sobeled, imageMat = image, grad_x,grad_y,abs_grad_x,abs_grad_y;	
		cvCvtColor(image,binaryImage,CV_RGB2GRAY);
		cvSobel(binaryImage,deepImage,1,1,1);
		showImage("Sobel image",deepImage);
	} else if (conversionMethod == ADAPTIVE_THRESHHOLD){
		IplImage *hsvImage = cvCreateImage(cvGetSize(image),image->depth,image->nChannels);
		cvCvtColor(image,hsvImage,CV_BGR2HSV);
		cvSplit(hsvImage,binaryImage,NULL,NULL,NULL);
		showImage("Hue image",binaryImage);
		cvAdaptiveThreshold(binaryImage,binaryImage,255,ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY,41,20);
		
		cvErode(binaryImage,binaryImage,0,4);
		cvNot(binaryImage,binaryImage);
		showImage("Adaptive Image", binaryImage);
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

	//showImage("Labeled image", data->labeledImage);
	cvWaitKey(0);
	return 0;
}
