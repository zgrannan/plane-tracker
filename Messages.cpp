#include "Messages.h"
#include "Log.h"
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

using namespace Messages;
using namespace std;
using namespace boost;

vector<char> RelativePositionMessage::toBytes() const {
  vector<char> byteVector;
  string byteString;
  int _pan = (int)this->pan;
  int _tilt= (int)this->tilt;
  string panS = (boost::format("%04d") % _pan).str();
  string tiltS = (boost::format("%04d") % _tilt).str();
  if (!positionLost){
    byteString = "#2 " + panS + " " + tiltS + "\n";
  } else {
    byteString = "";
  }
  return vector<char>(byteString.begin(),byteString.end()); 
}

vector<char> AbsolutePositionMessage::toBytes() const{
  vector<char> byteVector;
  string byteString;
  int _pan = (int)this->pan;
  int _tilt= (int)this->tilt;
  string panS = (boost::format("%04d") % _pan).str();
  string tiltS = (boost::format("%04d") % _tilt).str();
  if (!positionLost){
    byteString = "#1 " + panS + " " + tiltS + "\n";
  } else {
    byteString = "";
  }
  return vector<char>(byteString.begin(),byteString.end()); 
}

vector<int> PlaneVisionMessage::getDisplacement(){
  vector<int> resultVector;
  if (!hasPlane) return resultVector;
  if (!result) {
    Log::debug("Error: image is null");
    return resultVector;
  }
  double imageCenterX = result->width/ 2.0;
  double imageCenterY = result->height/ 2.0;
  double planeX = planeBlob.centroid.x;
  double planeY = planeBlob.centroid.y;
  int dx = planeX - imageCenterX;
  int dy = planeY - imageCenterY;
  resultVector.push_back(dx);
  resultVector.push_back(dy);
  return resultVector;
}
