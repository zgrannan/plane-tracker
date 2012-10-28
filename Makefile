CC=g++
LDFLAGS= -ggdb
CFLAGS= -c -Wall
MISC_FLAGS= -DTHERON_BOOST -O2
OBJECTS=$(SOURCES:.cpp=.o)

INCLUDE_FLAGS = \
  -I/usr/local/include/External \
  -I/usr/local/Cellar/opencv/2.4.2/include/opencv \
  -I/opt/X11/include

LIB_FLAGS = \
  `pkg-config opencv cvblob --cflags --libs` \
  -lopencv_core \
  -lboost_system-mt \
  -lboost_iostreams-mt \
  -ltherond \
  -lboost_thread-mt \
  -lopencv_highgui \
  -lopencv_imgproc \
  -lm \
  -L/usr/X11R6/lib \
  -lpthread \
  -lX11


SOURCES = \
  FrameAnalyzerActor.cpp \
  GPSReceiverInterface.cpp \
  GeoReference.cpp \
  GeoreferencingActor.cpp \
  Messages.cpp \
  MultimodalActor.cpp \
  Protocol.cpp \
  VideoReceiverInterface.cpp \
  Vision.cpp \
  tracker.cpp

EXECUTABLE=tracker

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(MISC_FLAGS) $(LDFLAGS) $(LIB_FLAGS) $(OBJECTS) -o $@

VisionTester: VisionTester.o FrameAnalyzerActor.o Vision.o Messages.o
	$(CC) $(MISC_FLAGS) $(LDFLAGS) $(LIB_FLAGS) VisionTester.o FrameAnalyzerActor.o Vision.o Messages.o -o $@

%.o: %.cpp
	$(CC) -c $(MISC_FLAGS) $(INCLUDE_FLAGS) $<  

clean :
	rm -rf *.o tracker *.dSYM
