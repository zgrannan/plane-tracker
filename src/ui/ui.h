#ifndef UI_H
#define UI_H

#include <QMainWindow>
#include <iostream>
#include <Theron/Theron.h>

using namespace std;

class FrameAnalyzerActor;
class MultimodalActor;
class GeoreferencingActor;

namespace Ui {
  class UI;
}


class UI : public QMainWindow
{
  Q_OBJECT

  public:
    explicit UI(QWidget *parent,
        FrameAnalyzerActor* frameAnalyzerActor,
        GeoreferencingActor* georeferencingActor,
        MultimodalActor* mulitimodalActor);
    ~UI();
    public slots:
    void selectPlaneColor(bool enabled);
    void toggleVideo(bool enabled);
    void toggleGPS(bool enabled);
    void updatePan(int pan);
    void updateTilt(int tilt);
    void updateAmplification(int amplification);
    void updateEdgeThresholding(int edgeThresholding);
    void updateMinBlobSize(int blobSize);
    void updateMaxBlobSize(int blobSize);
    void updatePositionWeight(int weight);
    void updateColorWeight(int weight);
    void updateSizeWeight(int weight);
    void updateRatioWeight(int weight);
    void setTrackerGPSFromFields();
    void setTrackerGPSFromRadio();

  protected:
    void keyPressEvent(QKeyEvent *e);

  private:
    FrameAnalyzerActor* frameAnalyzerActor;
    GeoreferencingActor* georeferencingActor;
    MultimodalActor* multimodalActor;
    Ui::UI *ui;
    int pan = 0, tilt = 0;
};

#endif // UI_H
