#-------------------------------------------------
#
# Project created by QtCreator 2012-11-03T20:37:05
#
#-------------------------------------------------

QT       += core gui

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS -= -O2

CONFIG += console debug
TARGET = PlaneTracker
TEMPLATE = app

INCLUDEPATH += /home/atom/Theron-5.01.01/Include
INCLUDEPATH += /usr/include/opencv

OBJECTS_DIR = ./obj
MOC_DIR = .moc
UI_DIR = src/ui

LIBS +=  `pkg-config opencv cvblob --cflags --libs` \
  -lopencv_core \
  -lboost_system \
  -lboost_iostreams \
  -ltherond \
  -lboost_date_time \
  -lboost_thread \
  -lboost_program_options \
  -lopencv_highgui \
  -lopencv_imgproc \
  -lm \
  -L/home/atom/Theron-5.01.01/Lib \
  -lpthread

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
