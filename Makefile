CC= g++

#Include debug symbols
LDFLAGS= -g

tracker : FrameAnalyzerActor.o GPSReceiverInterface.o GeoReference.o GeoreferencingActor.o Messages.o MultimodalActor.o Protocol.o VideoReceiverInterface.o Vision.o
	g++ `pkg-config opencv cvblob --cflags --libs`-I/usr/local/Cellar/opencv/2.4.2/include/opencv -lopencv_core -lopencv_highgui -lopencv_imgproc -ggdb -o tracker FrameAnalyzerActor.o GPSReceiverInterface.o GeoReference.o GeoreferencingActor.o Messages.o MultimodalActor.o Protocol.o VideoReceiverInterface.o Vision.o

FrameAnalyzerActor.o: Messages.h FrameAnalyzerActor.h FrameAnalyzerActor.cpp
	g++ -c FrameAnalyzerActor.cpp -DTHERON_BOOST -I/usr/local/include/External -I/usr/local/Cellar/opencv/2.4.2/include/opencv

GPSReceiverInterface.o: GPSReceiverInterface.h 
	g++ -c GPSReceiverInterface.cpp -DTHERON_BOOST -I/usr/local/include/External -I/usr/local/Cellar/opencv/2.4.2/include/opencv

GeoReference.o: GeoReference.h
	g++ -c GeoReference.cpp -DTHERON_BOOST -I/usr/local/include/External -I/usr/local/Cellar/opencv/2.4.2/include/opencv -I/usr/local/Cellar/opencv/2.4.2/include/opencv

Messages.o: Messages.h
	g++ -c Messages.h -DTHERON_BOOST -I/usr/local/include/External -I/usr/local/Cellar/opencv/2.4.2/include/opencv -lopencv_core -I/usr/local/Cellar/opencv/2.4.2/include/opencv

MultimodalActor.o: MultimodalActor.cpp
	g++ -c MultimodalActor.cpp -DTHERON_BOOST -I/usr/local/include/External -I/usr/local/Cellar/opencv/2.4.2/include/opencv

GeoreferencingActor.o: GeoreferencingActor.h
	g++ -c GeoreferencingActor.h -DTHERON_BOOST -I/usr/local/include/External -I/usr/local/Cellar/opencv/2.4.2/include/opencv

Protocol.o: Protocol.h
	g++ -c Protocol.h -DTHERON_BOOST -I/usr/local/include/External -I/usr/local/Cellar/opencv/2.4.2/include/opencv

VideoReceiverInterface.o: VideoReceiverInterface.h
	g++ -c VideoReceiverInterface.h -DTHERON_BOOST -I/usr/local/include/External -I/usr/local/Cellar/opencv/2.4.2/include/opencv

Vision.o: Vision.h
	g++ `pkg-config opencv cvblob --cflags --libs` -lopencv_core -lopencv_highgui -lopencv_imgproc -DTHERON_BOOST -I/usr/local/include/External -I/usr/local/Cellar/opencv/2.4.2/include/opencv -c Vision.h  -I/usr/local/Cellar/opencv/2.4.2/include/opencv
clean : 
	rm *.o *.gch 
