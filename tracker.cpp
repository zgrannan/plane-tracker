#include "VideoReceiverInterface.h"
#include "GPSReceiverInterface.h"
#include "FrameAnalyzerActor.h"
#include "GeoreferencingActor.h"
#include "MultiModalActor.h"
#include <Theron/Theron.h>

int main(int argc, char* argv[]){
	Theron::Framework framework;
	MultimodalActor multimodalActor(framework);
	FrameAnalyzerActor frameAnalyzerActor(framework,multimodalActor.GetAddress());
	GeoreferencingActor georeferencingActor(framework,32.0, -117.0, 0.0,multimodalActor.GetAddress());
	VideoReceiverInterface videoInterface(framework,frameAnalyzerActor.GetAddress());
	GPSReceiverInterface gpsReceiverInterface(framework, georeferencingActor.GetAddress());
}
