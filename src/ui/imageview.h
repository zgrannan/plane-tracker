#ifndef IMAGEVIEW_H 
#define IMAGEVIEW_H

#include <QMainWindow>
#include <cv.h>

namespace Ui {
  class ImageView;
}

class ImageView : public QMainWindow
{
  Q_OBJECT

  public: 
    explicit ImageView(QWidget *parent);
    void sendImage(IplImage* image){
      _sendImage(image);
    }
    ~ImageView();
  public slots:
    void updateImage(IplImage* image);
  signals:
    void _sendImage(IplImage* image);
  private:
    QImage iplImageToQImage(IplImage* image);
    Ui::ImageView *imageView;
};

#endif
