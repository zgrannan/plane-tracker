#-------------------------------------------------
#
# Project created by QtCreator 2012-11-03T20:37:05
#
#-------------------------------------------------

QT       += core gui

TARGET = PlaneTracker
TEMPLATE = app


INCLUDES += /usr/local/include/External 
INCLUDES += /usr/local/Cellar/opencv/2.4.2/include/opencv
INCLUDES += /usr/local/include/decklink

HEADERS += Log.cpp
HEADERS += FrameAnalyzerActor.cpp
HEADERS += GPSReceiverInterface.cpp
HEADERS += GeoReference.cpp
HEADERS += GeoreferencingActor.cpp
HEADERS += Messages.cpp
HEADERS += MultimodalActor.cpp
HEADERS += Protocol.cpp
HEADERS += VideoReceiverInterface.cpp
HEADERS += Vision.cpp
HEADERS += ui.h

SOURCES += Log.cpp
SOURCES += FrameAnalyzerActor.cpp
SOURCES += GPSReceiverInterface.cpp
SOURCES += GeoReference.cpp
SOURCES += GeoreferencingActor.cpp
SOURCES += Messages.cpp
SOURCES += MultimodalActor.cpp
SOURCES += Protocol.cpp
SOURCES += VideoReceiverInterface.cpp
SOURCES += Vision.cpp
SOURCES += tracker.cpp
SOURCES += ui.cpp

  `pkg-config opencv cvblob --cflags --libs` \

LIBS = \
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
FORMS   += ui.ui
