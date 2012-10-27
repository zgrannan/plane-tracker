#include "Messages.h"
#include <boost/lexical_cast.hpp>
using namespace Messages;
using namespace std;
using namespace boost;

vector<char> RelativePositionMessage::toBytes() const {
  string byteString = "RELATIVE " + lexical_cast<string>(this->pan) + " " + lexical_cast<string>(this->tilt) + "\r\n";
  vector<char> byteVector(byteString.begin(),byteString.end()); 
  return byteVector;
}

vector<char> AbsolutePositionMessage::toBytes() const{
  string byteString = "ABSOLUTE" + lexical_cast<string>(this->pan) + " " + lexical_cast<string>(this->tilt) + "\r\n";
  vector<char> byteVector(byteString.begin(),byteString.end()); 
  return byteVector;
}
