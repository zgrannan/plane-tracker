#-------------------------------------------------
#
# Project created by QtCreator 2012-11-03T20:37:05
#
#-------------------------------------------------

QT       += core gui

TARGET = PlaneTracker
TEMPLATE = app
CFLAGS = -c -Wall
DEFINES += -DTHERON_BOOST

INCLUDEPATH += /usr/local/include/External 
INCLUDEPATH += /usr/local/Cellar/opencv/2.4.2/include/opencv
INCLUDEPATH += /usr/local/include/decklink

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

  

LFLAGS = `pkg-config opencv cvblob --cflags --libs` 
LFLAGS += -lopencv_core
LFLAGS += -lboost_system-mt 
LFLAGS += -lboost_iostreams-mt
LFLAGS += -ltherond 
LFLAGS += -lboost_thread-mt 
LFLAGS += -lboost_program_options-mt 
LFLAGS += -lopencv_highgui 
LFLAGS += -lopencv_imgproc 
LFLAGS += -lm
LFLAGS += -lpthread 
LFLAGS += -framework CoreFoundation

FORMS   += ui.ui
