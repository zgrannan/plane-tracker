#include "src/util/Messages.h"
#include "src/util/Log.h"
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

using namespace Messages;
using namespace std;
using namespace boost;

vector<char> RelativePositionMessage::toBytes() const {
  vector<char> byteVector;
  string byteString;
  const int _pan = -(int)(this->pan * 100.0);
  const int _tilt= -(int)(this->tilt * 100.0);
  const string panS = (boost::format("%06d") % _pan).str();
  const string tiltS = (boost::format("%06d") % _tilt).str();
  if (!positionLost){
    byteString = "#1 " + panS + " " + tiltS + "\n";
  } else {
    byteString = "";
  }
  return vector<char>(byteString.begin(),byteString.end()); 
}

vector<char> AbsolutePositionMessage::toBytes() const{
  vector<char> byteVector;
  string byteString;
  const int _pan = -(int)(this->pan * 100.0);
  const int _tilt= -(int)(this->tilt * 100.0);
  const string panS = (boost::format("%06d") % _pan).str();
  const string tiltS = (boost::format("%06d") % _tilt).str();
  if (!positionLost){
    byteString = "#0 " + panS + " " + tiltS + "\n";
  } else {
    byteString = "";
  }
  return vector<char>(byteString.begin(),byteString.end()); 
}

vector<char> UseRSSIMessage::toBytes() const{
  const string byteString = "#2 000000 000000\n";
  return vector<char>(byteString.begin(),byteString.end());
}

vector<int> PlaneVisionMessage::getDisplacement() const{
  if (!hasPlane) return {};
  if (!result) {
    DEBUG("Error: image is null");
    return {};
  }
  const double imageCenterX = result->width/ 2.0;
  const double imageCenterY = result->height/ 2.0;
  const double planeX = planeBlob.centroid.x;
  const double planeY = planeBlob.centroid.y;
  const int dx = planeX - imageCenterX;
  const int dy = imageCenterY - planeY;
  return {dx,dy};
}
