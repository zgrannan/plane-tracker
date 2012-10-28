#include <iostream>
#include <Theron/Theron.h>
#include <cv.h>
#include <highgui.h>
#include <boost/algorithm/string.hpp>
#include <ctime>
#include "FrameAnalyzerActor.h"
#include "Messages.h"
#include "Vision.h"

using namespace std;
using namespace Messages;
using namespace cv;
using namespace Vision;
using namespace boost::algorithm;

int main(int argc, char* argv[]){
  Theron::Framework framework;
  Theron::Receiver imageReceiver;
  Theron::Receiver fakeMultimodalActor;
  Theron::Catcher<ImageMessage> imageCatcher;
  Theron::Address from;
  ImageMessage message(NULL);
  imageReceiver.RegisterHandler(&imageCatcher,&Theron::Catcher<ImageMessage>::Push);
  FrameAnalyzerActor frameAnalyzerActor(framework, imageReceiver.GetAddress(), fakeMultimodalActor.GetAddress());
  string input;
  while(true) {
    getline(cin,input);
    trim(input);
    if (input == "") break;
    IplImage* image = cvLoadImage(input.c_str());
    clock_t before = clock();
    framework.Send(ImageMessage(image), imageReceiver.GetAddress(), frameAnalyzerActor.GetAddress());
    imageReceiver.Wait();
    imageCatcher.Pop(message,from);
    showImage(message.image);
    clock_t after = clock();
    cerr <<"Analyzed image in: "<<(double)(after-before) / (double)CLOCKS_PER_SEC << " seconds\n";
    cvWaitKey(30);
  }
}
