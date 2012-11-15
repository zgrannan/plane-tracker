#include <DeckLinkAPI.h>
#include <DeckLinkAPIDispatch.cpp>
#include <cv.h>
#include <boost/thread.hpp>
#include <highgui.h>
#include <iostream>
#include "src/vision/VideoReceiverInterface.h"
#include "src/util/Messages.h"
#include "src/util/Log.h"

using namespace cv;
using namespace std;
using namespace Messages;

void VideoReceiverInterface::sendImage(IplImage* image){
  const ImageMessage message(image);
  Log::debug("Sending imageMessage to FrameAnalyzerActor");
  framework.Send(message,receiver.GetAddress(),frameAnalyzerActor);
}

class VideoCallback : public IDeckLinkInputCallback {
  public:
    VideoCallback(VideoReceiverInterface* vInterface): IDeckLinkInputCallback(),vInterface(vInterface){};
    virtual HRESULT VideoInputFrameArrived(IDeckLinkVideoInputFrame * videoFrame,
                                           IDeckLinkAudioInputPacket*) {
      long width = videoFrame->GetWidth();
      long height = videoFrame->GetHeight();
      BMDFrameFlags flags = videoFrame->GetFlags();
      if (flags == (unsigned int)bmdFrameHasNoInputSource){
        Log::debug("No input source connected\n");
        return S_OK;
      }
      IplImage* image = cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,3);
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
      //videoFrame->Release();
      Log::debug("Decklink Image received");
      return S_OK;
    }

    virtual HRESULT VideoInputFormatChanged(BMDVideoInputFormatChangedEvents,
        IDeckLinkDisplayMode*,
        BMDDetectedVideoInputFormatFlags){
      return S_OK;
    }
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID,LPVOID*) { return E_NOINTERFACE; }
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
  Log::debug("Creating decklink iterator...");
  if (deckLinkIterator == 0 ){
    cerr << "\n\tUnable to create DeckLink Iterator. Video analysis will be disabled.\n\n";
    return;
  }

  Log::debug("Creating decklink object...");
  if ( deckLinkIterator->Next(&deckLink) != S_OK ) {
    Log::error("\n\tCould not create decklink object. Video analysis will be disabled\n");
    return;
  }

  Log::debug("Querying decklink interface...");
  if ( deckLink->QueryInterface(IID_IDeckLinkInput, (void**)&deckLinkInput) != S_OK ) {
    Log::error("\n\tCould not find a decklink interface. Video analysis will be disabled\n");
    return;
  }

  Log::debug("Registering decklink input callback...");
  if ( deckLinkInput->SetCallback(new VideoCallback(this)) != S_OK ) {
    Log::error("\n\tCould not set the decklink callback. Video analysis will be disabled\n");
    return;
  }

  Log::debug("Enabling video input...");
  if ( deckLinkInput->EnableVideoInput(displayMode,pixelFormat,inputFlags) != S_OK ) {
    Log::error("\n\tCould not enable video input. Video analysis will be disabled\n");
    return;
  }
  
  Log::debug("Starting streams...");
  if ( deckLinkInput->StartStreams() != S_OK ) {
    Log::error("\n\tCould not start streams. Video analysis will be disabled\n");
    return;
  }
}

void VideoReceiverInterface::videoFunction(string videoFilename){
  VideoCapture capture(videoFilename);
  auto numFrames = capture.get(CV_CAP_PROP_FRAME_COUNT);
  Mat temp;
  IplImage* image;
  for(int i = 0; i < numFrames; i++) {
    capture >> temp;
    IplImage tempIpl = temp;
    image = new IplImage(tempIpl); 
    sendImage(cvCloneImage(image));
    usleep(41666);
  }
}

VideoReceiverInterface::VideoReceiverInterface(Theron::Framework &framework, Theron::Address frameAnalyzerActor): framework(framework), frameAnalyzerActor(frameAnalyzerActor) {
  cameraFunction();
}

VideoReceiverInterface::VideoReceiverInterface(Theron::Framework &framework, string videoFilename, Theron::Address frameAnalyzerActor): framework(framework), frameAnalyzerActor(frameAnalyzerActor) {
  boost::thread workerThread(&VideoReceiverInterface::videoFunction, this, videoFilename);
}
