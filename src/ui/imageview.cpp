#include "src/ui/imageview.h"
#include "src/ui/ui_imageview.h"
#include "src/util/Log.h"
#include <cv.h>
#include <boost/lexical_cast.hpp>
#include <QMouseEvent>

using namespace Messages;

ImageView::ImageView(QWidget *parent, int width, int height, double scale,
                     Theron::Address frameAnalyzerActor, Theron::Framework& framework):
                       QMainWindow(parent),
                       width(width),
                       height(height),
                       scale(scale),
                       frameAnalyzerActor(frameAnalyzerActor),
                       framework(framework),
                       imageView(new Ui::ImageView) {
  imageView->setupUi(this);
  resize(width,height);
  imageView->imageLabel->resize(width,height);
  connect(this,SIGNAL(_sendImage(IplImage*)), this, SLOT(updateImage(IplImage*)));
}

void ImageView::updateImage(IplImage* image){
  QImage qimage = iplImageToQImage(image);
  imageView->imageLabel->setPixmap(QPixmap::fromImage(qimage));
  cvReleaseImage(&image);
}

void ImageView::mouseDoubleClickEvent(QMouseEvent *event){
  Log::log("Mouse click: (" + boost::lexical_cast<string>(event->x()) +", " +
           boost::lexical_cast<string>(event->y()) + ")");
  double x = event->x() / scale;
  double y = event->y() / scale;
  framework.Send(BlobPositionMessage(x,y),
                receiver.GetAddress(),
                frameAnalyzerActor);
}
QImage ImageView::iplImageToQImage(IplImage* image) {
  int height = image->height;
  int width = image->width;

  const uchar *qImageBuffer = (const uchar*)image->imageData;
  QImage qimage(qImageBuffer, width, height, QImage::Format_RGB888);
  return qimage.rgbSwapped();
}

ImageView::~ImageView()
{
    delete imageView;
}
