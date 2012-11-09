#ifndef UI_H
#define UI_H

#include <QMainWindow>
#include <iostream>
#include <Theron/Theron.h>

using namespace std;

namespace Ui {
class UI;
}


class UI : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit UI(QWidget *parent,
                Theron::Framework& framework,
                Theron::Address frameAnalyzerActor,
                Theron::Address georeferencerActor,
                Theron::Address mulitimodalActor);
    ~UI();
public slots:
  void toggleVideo(bool enabled);
  void toggleGPS(bool enabled);
  void updatePan(int pan);
  void updateTilt(int tilt);
  void updateAmplification(int amplification);
    
private:
    Theron::Framework& framework;
    Theron::Actor frameAnalyzerActor;
    Theron::Actor georeferencerActor;
    Theron::Actor multimodalActor;
    Ui::UI *ui;
    int pan = 0, tilt = 0;
};

#endif // UI_H
