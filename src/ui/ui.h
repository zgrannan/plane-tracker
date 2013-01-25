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
        FrameAnalyzerActor* const frameAnalyzerActor,
        GeoreferencingActor* const georeferencingActor,
        MultimodalActor* const mulitimodalActor);
    ~UI();
    public slots:
    void selectPlaneColor(const bool enabled);
    void toggleVideo(const bool enabled);
    void toggleGPS(const bool enabled);
    void updatePan(const int pan);
    void updateTilt(const int tilt);
    void updateAmplification(const int amplification);
    void updateEdgeThresholding(const int edgeThresholding);
    void updateMinBlobSize(const int blobSize);
    void updateMaxBlobSize(const int blobSize);
    void updatePositionThresh(const int thresh);
    void updateColorThresh(const int thresh);
    void updateSizeThresh(const int thresh);
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
