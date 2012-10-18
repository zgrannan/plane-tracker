#include "VideoRecieverInterface.h"
#include "GPSRecieverInterface.h"
#include "FrameAnalyzerActor.h"
#include "GeoreferencingActor.h"
#include "MulitmodalActor.h"
#include <Theron/Theron.h>

int main(int argc, char* argv[]){
	Theron::Framework framework;
	MultimodalActor multimodalActor(framework);
	FrameAnalyzerActor frameAnalyzerActor(framework,multimodalActor.getAddress());
	GeoreferencingActor = georeferencingActor(framework,multimodalActor.getAddress());
	CvCapture* camera = cvCaptureFromCam(-1);
	VideoRecieverInterface* videoInterface = new VideoRecieverInterface(framework,camera,frameAnalyzerActor.getAddress());
	GPSRecieverInterface* gpsRecieverInterface = new GPSRecieverInterface(framework,serialConnection, georeferencingActor.getAddress());
	videoInterface->startListening();
	gpsRecieverInterface->startListening();
}
