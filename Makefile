CC=g++ -std=c++11
LDFLAGS= -ggdb
CFLAGS= -c -Wall
MISC_FLAGS= -DTHERON_BOOST
OBJECTS=$(SOURCES:.cpp=.o)

INCLUDE_FLAGS = \
  -I/usr/local/include/External \
  -I/usr/local/Cellar/opencv/2.4.2/include/opencv \
  -I/usr/local/include/decklink

LIB_FLAGS = \
  `pkg-config opencv cvblob --cflags --libs` \
  -lopencv_core \
  -lboost_system-mt \
  -lboost_iostreams-mt \
  -ltherond \
  -lboost_thread-mt \
  -lboost_program_options-mt \
  -lopencv_highgui \
  -lopencv_imgproc \
  -lm \
  -lpthread \
  -framework CoreFoundation


SOURCES = \
  Log.cpp \
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

%.o: %.cpp
	$(CC) -c $(MISC_FLAGS) $(INCLUDE_FLAGS) $<  

clean :
	rm -rf *.o tracker *.dSYM
