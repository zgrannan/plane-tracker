#include "src/ui/imageview.h"
#include "src/ui/ui_imageview.h"
#include <cv.h>

ImageView::ImageView(QWidget *parent): QMainWindow(parent), imageView(new Ui::ImageView) {
  imageView->setupUi(this);
  connect(this,SIGNAL(_sendImage(IplImage*)), this, SLOT(updateImage(IplImage*)));
}

void ImageView::updateImage(IplImage* image){
  QImage qimage = iplImageToQImage(image);
  imageView->imageLabel->setPixmap(QPixmap::fromImage(qimage));
  cvReleaseImage(&image);
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
