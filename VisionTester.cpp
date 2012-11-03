#include <iostream>
#include <Theron/Theron.h>
#include <cv.h>
#include <highgui.h>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <DeckLinkAPI.h>
#include <DeckLinkAPIDispatch.cpp>
#include <ctime>
#include "Messages.h"
#include "Vision.h"

using namespace std;
using namespace Messages;
using namespace cv;
using namespace Vision;
using namespace boost::algorithm;

#define CAMERA_H_FOV 60.0
#define CAMERA_V_FOV 45.0

void calculateRelativePosition(IplImage* image){
  vector<VisualPlaneData> previousPlanes;
  vector<int> skyHSV;
  vector<int> planeHSV;

  VisualPlaneData data = findPlane(image,previousPlanes,skyHSV,planeHSV);
  if (data.hasPlane){
    previousPlanes.push_back(data);
    double dx = data.getDisplacement()[0];
    double dy = data.getDisplacement()[1];
    double centerX = data.image->width / 2;
    double centerY = data.image->height/ 2;
    double pan = dx / data.image->width * CAMERA_H_FOV;
    double tilt = dy / data.image->height * CAMERA_V_FOV;
    CvPoint origin = cvPoint(centerX,centerY);
    CvPoint destination =  cvPoint(centerX+dx,centerY+dy);
    CvScalar color = cvScalar(0,0,255);
    cvLine(image,origin,destination,color,2);
  }
}

class VideoCallback : public IDeckLinkInputCallback {
  virtual HRESULT VideoInputFrameArrived(IDeckLinkVideoInputFrame * videoFrame,
      IDeckLinkAudioInputPacket * audioPacket) {
    long width = videoFrame->GetWidth();
    long height = videoFrame->GetHeight();
    BMDFrameFlags flags = videoFrame->GetFlags();
    cerr <<"Image width: "<<width<<" height: "<<height<<endl;
    if (flags == bmdFrameHasNoInputSource) return S_OK;
    IplImage* image= cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,3);
    void* _buffer;
    videoFrame->GetBytes(&_buffer);
    unsigned char* buffer = (unsigned char*) _buffer;
    for(int i = 0, j=0; i < width * height * 3; i+=6, j+=4)
    { 
      unsigned char u = buffer[j];
      unsigned char y = buffer[j+1];
      unsigned char v = buffer[j+2];

      image->imageData[i+2] = 1.0*y + 8 + 1.402*(v-128);
      image->imageData[i+1] = 1.0*y - 0.34413*(u-128) - 0.71414*(v-128);
      image->imageData[i] = 1.0*y + 1.772*(u-128) + 0;

      y = buffer[j+3];
      image->imageData[i+5] = 1.0*y + 8 + 1.402*(v-128);
      image->imageData[i+4] = 1.0*y - 0.34413*(u-128) - 0.71414*(v-128);
      image->imageData[i+3] = 1.0*y + 1.772*(u-128) + 0;
    }
  cvSaveImage("image.jpg",image);
  exit(0);
  cerr << "Received a frame!\n";                               
  return S_OK;
}
virtual HRESULT VideoInputFormatChanged(BMDVideoInputFormatChangedEvents notifactionEvents,
    IDeckLinkDisplayMode * newDisplayMode,
    BMDDetectedVideoInputFormatFlags detectedSignalFlags){
  return S_OK;
}
virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, LPVOID *ppv) { return E_NOINTERFACE; }
virtual ULONG STDMETHODCALLTYPE AddRef(void){return 0;};
virtual ULONG STDMETHODCALLTYPE  Release(void){return 0;};
};

int main(int argc, char* argv[]){
  Mat temp;
  IDeckLink *deckLink;
  IDeckLinkInput *deckLinkInput;
  BMDVideoInputFlags inputFlags = 0;
  BMDDisplayMode displayMode = bmdModeNTSC;
  BMDPixelFormat pixelFormat = bmdFormat8BitYUV;
  IDeckLinkIterator *deckLinkIterator = CreateDeckLinkIteratorInstance();
  cerr <<"Created deckling iterator\n";
  HRESULT result = deckLinkIterator->Next(&deckLink);
  cerr <<"Created decklink object\n";
  deckLink->QueryInterface(IID_IDeckLinkInput, (void**)&deckLinkInput);
  cerr <<"Created decklinkInput object\n";
  deckLinkInput->SetCallback(new VideoCallback());
  cerr <<"Registered decklinkInput callback\n";
  deckLinkInput->EnableVideoInput(displayMode,pixelFormat,inputFlags);
  cerr <<"Enabling video input\n";
  deckLinkInput->StartStreams();
  cerr <<"Starting stream\n";
  VideoCapture capture;
  VideoWriter writer;
  if ( argc > 1 ){
    capture.open(argv[1]);
  } 
  string input;
  IplImage* image;
  IplImage tempIpl;
  int frame = 1000;
  while(true) {
    frame++;
    if (argc == 1){
      //getline(cin,input);
      //trim(input);
      //if (input == "") break;
      //image = cvLoadImage(input.c_str());
      cvWaitKey(50);
      continue;
    } else {
      capture >> temp;
      IplImage tempIpl = temp;
      image = new IplImage(tempIpl);
      //cvWaitKey(50);
    }
    clock_t before = clock();
    calculateRelativePosition(image);
    showImage("Result", image);
    const char* filename = (string("output/") + boost::lexical_cast<string>(frame) + string(".jpg")).c_str();
    cvSaveImage(filename,image);
    cout <<"Wrote frame\n";
    cvWaitKey(30);
    clock_t after = clock();
    cerr <<"Analyzed image in: "<<(double)(after-before) / (double)CLOCKS_PER_SEC << " seconds\n";
  }
}
