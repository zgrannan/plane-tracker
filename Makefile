CC= g++

#Include debug symbols
LDFLAGS= -g

tracker : FrameAnalyzerActor.o GPSRecieverInterface.o GeoReference.o GeoreferencingActor.o ListeningInterface.o Messages.o MultimodalActor.o Protocol.o VideoRecieverInterface.o vision.o
  g++ `pkg-config opencv cvblob --cflags --libs`-I/usr/local/Cellar/opencv/2.4.2/include/opencv -lopencv_core -lopencv_highgui -lopencv_imgproc -ggdb -o tracker FrameAnalyzerActor.o GPSRecieverInterface.o GeoReference.o GeoreferencingActor.o ListeningInterface.o Messages.o MultimodalActor.o Protocol.o VideoRecieverInterface.o vision.o

FrameAnalyzerActor.o: FrameAnalyzerActor.h
  g++ -c FrameAnalyzerActor.o

GPSRecieverInterface.o: GPSRecieverInterface.h
  g++ -c GPSRecieverInterface.o

Georeference.o: Georeference.h
  g++ -c Georeference.o

ListeningInterface.o: ListeningInterface.h
  g++ -c ListeningInterface.o

Messages.o: Messages.h
  g++ -c Messages.o

MultimodalActor.o: MultimodalActor.h
  g++ -c MultimodalActor.o

GeoreferencingActor.o: GeoreferencingActor.h
  g++ -c GeoreferencingActor.o

Protocol.o: Protocol.h
  g++ -c Protocol.o

VideoRecieverInterface.o: VideoRecieverInterface.h
  g++ -c VideoRecieverInterface.o

vision.o: vision.h
  g++ -c vision.o
clean : 
  rm *.o tracker
