#include <iostream>
#include <Theron/Theron.h>
#include <cv.h>
#include <highgui.h>
#include "FrameAnalyzerActor.h"
#include "Messages.h"

using namespace std;
using namespace Messages;
using namespace cv;

int main(int argc, char* argv[]){
  namedWindow("Display window", CV_WINDOW_AUTOSIZE);
  Theron::Framework framework;
  Theron::Receiver receiver;
  FrameAnalyzerActor frameAnalyzerActor(framework, receiver.GetAddress());
  string input;
  while(true) {
    getline(cin,input);
    if (input == "") break;
    IplImage* image = cvLoadImage(input.c_str());
    framework.Send(ImageMessage(image), receiver.GetAddress(), frameAnalyzerActor.GetAddress());
  }
}
