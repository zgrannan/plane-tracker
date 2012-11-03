#include <DeckLinkAPI.h>
#include <DeckLinkAPIDispatch.cpp>
#include <cv.h>
#include <boost/thread.hpp>
#include <highgui.h>
#include <iostream>
#include "VideoReceiverInterface.h"
#include "Messages.h"
using namespace cv;
using namespace std;
using namespace Messages;

void VideoReceiverInterface::sendImage(IplImage* image){
  const ImageMessage message(cvCloneImage(image));
  cerr <<"Sending message";
  framework.Send(message,receiver.GetAddress(),frameAnalyzerActor);
  cerr <<"Sent";
}

class VideoCallback : public IDeckLinkInputCallback {
  public:
  VideoCallback(VideoReceiverInterface* vInterface): vInterface(vInterface), IDeckLinkInputCallback(){};
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
  vInterface->sendImage(image);
  cerr <<"Image received";
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
private:
VideoReceiverInterface* vInterface;
};

void VideoReceiverInterface::cameraFunction(){
  IDeckLink *deckLink;
  IDeckLinkInput *deckLinkInput;
  BMDVideoInputFlags inputFlags = 0;
  BMDDisplayMode displayMode = bmdModeHD1080i5994;
  BMDPixelFormat pixelFormat = bmdFormat8BitYUV;
  IDeckLinkIterator *deckLinkIterator = CreateDeckLinkIteratorInstance();
  cerr <<"Created deckling iterator\n";
  HRESULT result = deckLinkIterator->Next(&deckLink);
  cerr <<"Created decklink object\n";
  deckLink->QueryInterface(IID_IDeckLinkInput, (void**)&deckLinkInput);
  cerr <<"Created decklinkInput object\n";
  deckLinkInput->SetCallback(new VideoCallback(this));
  cerr <<"Registered decklinkInput callback\n";
  deckLinkInput->EnableVideoInput(displayMode,pixelFormat,inputFlags);
  cerr <<"Enabling video input\n";
  deckLinkInput->StartStreams();
  cerr <<"Starting stream\n";
}

void VideoReceiverInterface::videoFunction(string videoFilename){
  VideoCapture capture(videoFilename);
  Mat temp;
  IplImage* image;
  while (true) {
    capture >> temp;
    IplImage tempIpl = temp;
    image = new IplImage(tempIpl); 
    sendImage(image);
    usleep(30000);
  }
}

VideoReceiverInterface::VideoReceiverInterface(Theron::Framework &framework, Theron::Address frameAnalyzerActor): framework(framework), frameAnalyzerActor(frameAnalyzerActor) {
  cameraFunction();
}

VideoReceiverInterface::VideoReceiverInterface(Theron::Framework &framework, string videoFilename, Theron::Address frameAnalyzerActor): framework(framework), frameAnalyzerActor(frameAnalyzerActor) {
  boost::thread workerThread(&VideoReceiverInterface::videoFunction, this, videoFilename);
}
