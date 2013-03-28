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
    explicit ImageView(QWidget *parent, const int width, const int height, const double scale,
                       const Theron::Address frameAnalyzerActor, Theron::Framework &framework);
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
    QImage iplImageToQImage(const IplImage* image) const;
    const int width, height;
    const double scale;
    const Theron::Address frameAnalyzerActor;
    Theron::Framework& framework;
    Ui::ImageView *imageView;
    const Theron::Receiver receiver;
};

#endif
