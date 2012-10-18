#include <cv.h>

#define CAMERA_H_FOV 60
#define CAMERA_V_FOV 30

/**
 * PlaneTracker controller
 */
k

int main(int argc, char** argv){
	int cameraIndex = -1;
	if (argc > 1){
		cameraIndex = atoi(argv[1]);
	}
	CvCapture* cameraInput = cvCaptureFromCam(cameraIndex);
	IplImage *image;
	list<VisualPlaneData*>* seenPlanes = new list();
	VisualPlaneData* foundPlane;
	vector<int>* skyHSV= new vector<int>();
	vector<int>* planeHSV = new vector<int>();
	while (image = cvQueryFrame(cameraInput)){
		VisualPlaneData* data = findPlane(image,seenPlanes,skyHSV,planeHSV);
		if (data){
			seenPlanes.push_front(data);
			vector*<int> displacement = data->getDisplacement();
			vector*<float> displ
		} 
	}
}
