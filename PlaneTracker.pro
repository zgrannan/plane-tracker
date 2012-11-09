#-------------------------------------------------
#
# Project created by QtCreator 2012-11-03T20:37:05
#
#-------------------------------------------------

QT       += core gui

CONFIG += console
TARGET = PlaneTracker
TEMPLATE = app

DEFINES += THERON_BOOST

INCLUDEPATH += /usr/local/include/External 
INCLUDEPATH += /usr/local/Cellar/opencv/2.4.2/include/opencv
QMAKE_CXXFLAGS += -isystem /usr/local/include/decklink

LIBS +=  `pkg-config opencv cvblob --cflags --libs` \
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

HEADERS += Log.h
HEADERS += FrameAnalyzerActor.h
HEADERS += GPSReceiverInterface.h
HEADERS += GeoReference.h
HEADERS += GeoreferencingActor.h
HEADERS += Messages.h
HEADERS += MultimodalActor.h
HEADERS += Protocol.h
HEADERS += VideoReceiverInterface.h
HEADERS += Vision.h
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

FORMS   += ui.ui
