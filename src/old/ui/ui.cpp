#include "src/util/Log.h"
#include "src/util/Messages.h"
#include "src/vision/FrameAnalyzerActor.h"
#include "src/gps/GeoreferencingActor.h"
#include "src/main/MultimodalActor.h"
#include "src/ui/ui.h"
#include "src/ui/ui_ui.h"
#include <boost/lexical_cast.hpp>
#include <Theron/Theron.h>
#include <QKeyEvent>

using namespace Messages;
using namespace Qt;

UI::UI( QWidget *parent,
        FrameAnalyzerActor* const frameAnalyzerActor,
        GeoreferencingActor* const georeferencingActor,
        MultimodalActor* const multimodalActor):
    QMainWindow(parent),
    frameAnalyzerActor(frameAnalyzerActor),
    georeferencingActor(georeferencingActor),
    multimodalActor(multimodalActor),
    ui(new Ui::UI) {

      ui->setupUi(this);
      QObject::connect(ui->toggleVideoButton,SIGNAL(clicked(bool)),this,SLOT(toggleVideo(bool)));
      QObject::connect(ui->toggleGPSButton,SIGNAL(clicked(bool)),this,SLOT(toggleGPS(bool)));
      QObject::connect(ui->panSlider, SIGNAL(valueChanged(int)),this,SLOT(updatePan(int)));
      QObject::connect(ui->tiltSlider, SIGNAL(valueChanged(int)),this,SLOT(updateTilt(int)));
      QObject::connect(ui->amplificationSlider,
                       SIGNAL(valueChanged(int)),
                       this,
                       SLOT(updateAmplification(int)));

      QObject::connect(ui->edgeThresholdingSlider,
                       SIGNAL(valueChanged(int)),
                       this,
                       SLOT(updateEdgeThresholding(int)));

      QObject::connect(ui->maxBlobSlider,
                       SIGNAL(valueChanged(int)),
                       this,
                       SLOT(updateMaxBlobSize(int)));
                       
      QObject::connect(ui->minBlobSlider,
                      SIGNAL(valueChanged(int)),
                      this,
                      SLOT(updateMinBlobSize(int)));

      QObject::connect(ui->planeColorButton,
                       SIGNAL(clicked(bool)),
                       this,
                       SLOT(selectPlaneColor(bool)));

      QObject::connect(ui->colorThreshSlider,
                       SIGNAL(valueChanged(int)),
                       this,
                       SLOT(updateColorThresh(int)));

      QObject::connect(ui->positionThreshSlider,
                       SIGNAL(valueChanged(int)),
                       this,
                       SLOT(updatePositionThresh(int)));

      QObject::connect(ui->sizeThreshSlider,
                       SIGNAL(valueChanged(int)),
                       this,
                       SLOT(updateSizeThresh(int)));

      QObject::connect(ui->useRadioPosition,
                       SIGNAL(clicked()),
                       this,
                       SLOT(setTrackerGPSFromRadio()));

      QObject::connect(ui->useTextPosition,
                       SIGNAL(clicked()),
                       this,
                       SLOT(setTrackerGPSFromFields()));
}

void UI::setTrackerGPSFromFields(){
  const double lat = boost::lexical_cast<double>(ui->trackerLatitudeTextEdit->toPlainText().toStdString());
  const double lon = boost::lexical_cast<double>(ui->trackerLongitudeTextEdit->toPlainText().toStdString());
  const double alt= boost::lexical_cast<double>(ui->trackerAltitudeTextEdit->toPlainText().toStdString());
  georeferencingActor->setPosition(lat,lon,alt);
}
void UI::setTrackerGPSFromRadio(){
  georeferencingActor->setPositionFromRadio();
}
void UI::keyPressEvent(QKeyEvent * const e){
  int value;
  const int step = 5;
  switch(e->key()){
  case Key::Key_A: 
    value = ui->panSlider->value();
    ui->panSlider->setValue(value-step);
  break;
  case Key::Key_D: 
    value = ui->panSlider->value();
    ui->panSlider->setValue(value+step);
  break;
  case Key::Key_W: 
    value = ui->tiltSlider->value();
    ui->tiltSlider->setValue(value+step);
  break;
  case Key::Key_S: 
    value = ui->tiltSlider->value();
    ui->tiltSlider->setValue(value-step);
  break;
  }
}

void UI::updateSizeThresh(const int thresh){
  frameAnalyzerActor->setSizeThresh(thresh);
}

void UI::updatePositionThresh(const int thresh){
  frameAnalyzerActor->setPositionThresh(thresh);
}

void UI::updateColorThresh(const int thresh){
  frameAnalyzerActor->setColorThresh(thresh);
}

void UI::selectPlaneColor(const bool enabled){
  if (enabled){
    frameAnalyzerActor->selectColor();
    DEBUG("Sent \"Select Color\" message");
  } else {
    frameAnalyzerActor->deselectColor();
    DEBUG("Sent \"Deselect Color\" message");
  }
}

void UI::toggleVideo(const bool disabled){
  if (disabled){
    frameAnalyzerActor->disable();
    Log::log("Video disabled.\n");
  } else {
    frameAnalyzerActor->enable();
    Log::log("Video enabled.\n");
  }
}

void UI::toggleGPS(const bool disabled) {
  if (disabled){
    georeferencingActor->disable();
    Log::log("GPS disabled.\n");
  } else {
    georeferencingActor->enable();
    Log::log("GPS enabled.\n");
  }
}

void UI::updatePan(const int pan){
  this->pan = pan;
  multimodalActor->instructGimbal(AbsolutePositionMessage(pan,tilt));
  DEBUG("Changing pan to: "+ boost::lexical_cast<string>(pan));
}

void UI::updateTilt(const int tilt){
  this->tilt = tilt;
  multimodalActor->instructGimbal(AbsolutePositionMessage(pan,tilt));
  DEBUG("Changing tilt to: " + boost::lexical_cast<string>(tilt));
}

void UI::updateAmplification(const int amplification){
  DEBUG("Changing amplification to: "+ boost::lexical_cast<string>(amplification));
  multimodalActor->setAmplification((double)(amplification/10.0) + 1.0 );
}

void UI::updateEdgeThresholding(const int thresholding){
  DEBUG("Changing edge thresholding to: " + boost::lexical_cast<string>(thresholding));
  frameAnalyzerActor->setEdgeThresholding(thresholding);
}

void UI::updateMinBlobSize(const int blobSize){
  DEBUG("Changing minimum blob size to : " + boost::lexical_cast<string>(blobSize));
  frameAnalyzerActor->setMinBlobSize(blobSize);
}

void UI::updateMaxBlobSize(const int blobSize){
  DEBUG("Changing maximum blob size to : " + boost::lexical_cast<string>(blobSize));
  frameAnalyzerActor->setMaxBlobSize(blobSize);
}

UI::~UI()
{
    delete frameAnalyzerActor;
    delete georeferencingActor;
    delete multimodalActor;
    delete ui;
}
