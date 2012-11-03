#include "Messages.h"
#include <boost/lexical_cast.hpp>
using namespace Messages;
using namespace std;
using namespace boost;

vector<char> RelativePositionMessage::toBytes() const {
  vector<char> byteVector;
  string byteString;
  if (!positionLost){
    byteString = "RELATIVE " + lexical_cast<string>(this->pan) + " " + lexical_cast<string>(this->tilt) + "\r\n";
  } else {
    byteString = "VIDEO LOST\r\n";
  }
  return vector<char>(byteString.begin(),byteString.end()); 
}

vector<char> AbsolutePositionMessage::toBytes() const{
  vector<char> byteVector;
  string byteString;
  if (!positionLost){
    byteString = "RELATIVE " + lexical_cast<string>(this->pan) + " " + lexical_cast<string>(this->tilt) + "\r\n";
  } else {
    byteString = "GPS LOST\r\n";
  }
  return vector<char>(byteString.begin(),byteString.end()); 
}

vector<int> PlaneVisionMessage::getDisplacement(){
  cerr <<"Calculating displacement...\n";
  vector<int> resultVector;
  if (!hasPlane) return resultVector;
  if (!result) {
    cerr <<"Error: image is null\n";
    return resultVector;
  }
  cerr<<"about to calculate center\n";
  double imageCenterX = result->width/ 2.0;
  double imageCenterY = result->height/ 2.0;
  cerr<<"Calculated image center\n";
  double planeX = planeBlob.centroid.x;
  double planeY = planeBlob.centroid.y;
  cerr<<"Calculated plane centroid\n";
  int dx = planeX - imageCenterX;
  int dy = planeY - imageCenterY;
  resultVector.push_back(dx);
  resultVector.push_back(dy);
  cerr<<"Complete\n";
  return resultVector;
}
