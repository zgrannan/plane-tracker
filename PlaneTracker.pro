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
INCLUDEPATH += /Users/zgrannan/opencv_trunk/include/opencv
QMAKE_CXXFLAGS += -isystem /usr/local/include/decklink

OBJECTS_DIR = ./obj
MOC_DIR = .moc
UI_DIR = src/ui

LIBS +=  `pkg-config opencv cvblob --cflags --libs` \
  -lopencv_core \
  -lboost_system-mt \
  -lboost_iostreams-mt \
  -ltherond \
  -lboost_date_time-mt \
  -lboost_thread-mt \
  -lboost_program_options-mt \
  -lopencv_highgui \
  -lopencv_imgproc \
  -lm \
  -lpthread \
  -framework CoreFoundation \

HEADERS += src/util/Log.h
HEADERS += src/vision/FrameAnalyzerActor.h
HEADERS += src/gps/GPSReceiverInterface.h
HEADERS += src/gps/GeoReference.h
HEADERS += src/gps/GeoreferencingActor.h
HEADERS += src/util/Messages.h
HEADERS += src/main/MultimodalActor.h
HEADERS += src/gps/Protocol.h
HEADERS += src/vision/VideoReceiverInterface.h
HEADERS += src/vision/Vision.h
HEADERS += src/ui/ui.h
HEADERS += src/ui/imageview.h

SOURCES += src/util/Log.cpp
SOURCES += src/vision/FrameAnalyzerActor.cpp
SOURCES += src/gps/GPSReceiverInterface.cpp
SOURCES += src/gps/GeoReference.cpp
SOURCES += src/gps/GeoreferencingActor.cpp
SOURCES += src/util/Messages.cpp
SOURCES += src/main/MultimodalActor.cpp
SOURCES += src/gps/Protocol.cpp
SOURCES += src/vision/VideoReceiverInterface.cpp
SOURCES += src/vision/Vision.cpp
SOURCES += src/ui/ui.cpp
SOURCES += src/ui/imageview.cpp
SOURCES += src/main/tracker.cpp

FORMS   += src/ui/ui.ui
FORMS   += src/ui/imageview.ui
