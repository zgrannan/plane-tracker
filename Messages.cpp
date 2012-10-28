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
