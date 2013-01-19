#ifndef IMAGEVIEW_H 
#define IMAGEVIEW_H

#include "src/util/Messages.h"
#include <QMainWindow>
#include <cv.h>
#include <Theron/Theron.h>

namespace Ui {
  class ImageView;
}

class ImageView : public QMainWindow
{
  Q_OBJECT

  public: 
    explicit ImageView(QWidget *parent, int width, int height, double scale,
                       Theron::Address frameAnalyzerActor, Theron::Framework &framework);
    void sendImage(IplImage* image){
      _sendImage(image);
    }
    ~ImageView();
  public slots:
    void updateImage(IplImage* image);
  signals:
    void _sendImage(IplImage* image);
  protected:
    void mouseReleaseEvent(QMouseEvent *event);
  private:
    QImage iplImageToQImage(IplImage* image);
    int width, height;
    double scale;
    Theron::Address frameAnalyzerActor;
    Theron::Framework& framework;
    Ui::ImageView *imageView;
    Theron::Receiver receiver;
};

#endif
