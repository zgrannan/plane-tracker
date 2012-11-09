#include "ui.h"
#include "ui_ui.h"
#include "Log.h"
#include "Messages.h"
#include <boost/lexical_cast.hpp>
#include <Theron/Theron.h>

using namespace Messages;

UI::UI( QWidget *parent,
        Theron::Address frameAnalyzerActor,
        Theron::Address georeferencingActor,
        Theeron::Address mulitimodalActor):
    QMainWindow(parent),
    framework(framework),
    frameAnalyzerActorActor(frameAnalyzer),
    georeferencingActor(georeferencingActor),
    multimodalActor(multimodalActor),
    ui(new Ui::UI) {
    ui->setupUi(this);
      QObject::connect(ui->toggleVideoButton,SIGNAL(clicked(bool)),this,SLOT(toggleVideo(bool)));
      QObject::connect(ui->toggleGPSButton,SIGNAL(clicked(bool)),this,SLOT(toggleGPS(bool)));
      QObject::connect(ui->panSlider, SIGNAL(valueChanged(int)),this,SLOT(updatePan(int)));
      QObject::connect(ui->tiltSlider, SIGNAL(valueChanged(int)),this,SLOT(updateTilt(int)));
      QObject::connect(ui->amplificationSlider, SIGNAL(valueChanged(int)),this,SLOT(updateAmplification(int)));
}


void UI::toggleVideo(bool disabled){
  if (disabled){
    framework.Send(DisableMessage(), receiver.getAddress(), frameAnalyzerActor);
    Log::log("Video disabled.\n");
  } else {
    framework.Send(EnableMessage(), receiver.getAddress(), frameAnalyzerActor);
    Log::log("Video enabled.\n");
  }
}

void UI::toggleGPS(bool disabled) {
  if (disabled){
    framework.Send(DisableMessage(), receiver.getAddress(), georeferencingActor);
    Log::log("GPS disabled.\n");
  } else {
    framework.Send(EnableMessage(), receiver.getAddress(), georeferencingActor);
    Log::log("GPS enabled.\n");
  }
}

void UI::updatePan(int pan){
  this->pan = pan;
  framework.Send(AbsolutePositionMessage(pan,tilt), receiver.getAddress(), multimodalActor);
  Log::debug("Changing pan to: "+ boost::lexical_cast<string>(pan));
}

void UI::updateTilt(int tilt){
  this->tilt = tilt;
  framework.Send(AbsolutePositionMessage(pan,tilt), receiver.getAddress(), multimodalActor);
  Log::debug("Changing tilt to: " + boost::lexical_cast<string>(tilt));
}

void UI::updateAmplification(int amplification){
  framework.Send(AmplificationMessage(amplification), receiver.getAddress(), multimodalActor);
  Log::debug("Changing amplification to: "+ boost::lexical_cast<string>(amplification));
}

UI::~UI()
{
    delete ui;
}
