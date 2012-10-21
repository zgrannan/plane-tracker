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
	VideoRecieverInterface* videoInterface = new VideoRecieverInterface(framework,frameAnalyzerActor.getAddress());
	GPSRecieverInterface* gpsRecieverInterface = new GPSRecieverInterface(framework, georeferencingActor.getAddress());
}
