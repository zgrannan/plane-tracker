CC= g++

#Include debug symbols
LDFLAGS= -g

tracker :
	g++ FrameAnalyzerActor.cpp GPSReceiverInterface.cpp GeoReference.cpp GeoreferencingActor.cpp Messages.cpp MultimodalActor.cpp Protocol.cpp VideoReceiverInterface.cpp Vision.cpp tracker.cpp -I/usr/local/include/External -DTHERON_BOOST `pkg-config opencv cvblob --cflags --libs`-I/usr/local/Cellar/opencv/2.4.2/include/opencv -lopencv_core -lboost_system-mt -ltherond -lboost_thread-mt -lopencv_highgui -lopencv_imgproc -ggdb -o tracker 

