CC= g++

#Include debug symbols
LDFLAGS= -g

tracker : FrameAnalyzerActor.o GPSRecieverInterface.o GeoReference.o GeoreferencingActor.o Messages.o MultimodalActor.o Protocol.o VideoRecieverInterface.o Vision.o
	g++ `pkg-config opencv cvblob --cflags --libs`-I/usr/local/Cellar/opencv/2.4.2/include/opencv -lopencv_core -lopencv_highgui -lopencv_imgproc -ggdb -o tracker FrameAnalyzerActor.o GPSRecieverInterface.o GeoReference.o GeoreferencingActor.o Messages.o MultimodalActor.o Protocol.o VideoRecieverInterface.o Vision.o

FrameAnalyzerActor.o: Messages.h FrameAnalyzerActor.h
	g++ -c FrameAnalyzerActor.h -DTHERON_BOOST -I/usr/local/include/External

GPSRecieverInterface.o: GPSRecieverInterface.h
	g++ -c GPSRecieverInterface.h -DTHERON_BOOST -I/usr/local/include/External 

GeoReference.o: GeoReference.h
	g++ -c GeoReference.h -DTHERON_BOOST -I/usr/local/include/External -I/usr/local/Cellar/opencv/2.4.2/include/opencv

Messages.o: Messages.h
	g++ -c Messages.h -DTHERON_BOOST -I/usr/local/include/External

MultimodalActor.o: MultimodalActor.h
	g++ -c MultimodalActor.h -DTHERON_BOOST -I/usr/local/include/External

GeoreferencingActor.o: GeoreferencingActor.h
	g++ -c GeoreferencingActor.h -DTHERON_BOOST -I/usr/local/include/External

Protocol.o: Protocol.h
	g++ -c Protocol.h -DTHERON_BOOST -I/usr/local/include/External

VideoRecieverInterface.o: VideoRecieverInterface.h
	g++ -c VideoRecieverInterface.h -DTHERON_BOOST -I/usr/local/include/External

Vision.o: Vision.h
	g++ `pkg-config opencv cvblob --cflags --libs` -lopencv_core -lopencv_highgui -lopencv_imgproc -c Vision.h -DTHERON_BOOST -I/usr/local/include/External -I/usr/local/Cellar/opencv/2.4.2/include/opencv

clean : 
	rm *.o tracker
