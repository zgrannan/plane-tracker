#include <iostream>
#include "src/arguments.h"
#include "src/planeTrackerClient.h"

using namespace std;

namespace PlaneTracking {

  using namespace PlaneTracking;

  void sleepUntilExit(){
    char temp;
    do {
      cin >> temp;
    } while ( temp !='');
  }

  unique_ptr<PlaneTrackerClient> createClient(int argc, char** argv){
    PlaneTrackerClientArgs clientArguments(argc, argv);
    unique_ptr<PlaneTrackerClient> client = new PlaneTrackerClient(clientArguments);
    client->run();
    return client;
  }

  void reportInvalidArguments(int argc, char** argv) {
    auto invalidArguments = PlaneTrackerClientArgs::invalidArguments(argc, argv);
    cout << "Invalid arguments: \n";
    for ( auto argument : invalidArguments){
      cout << "\t" << argument << "\n";
    }
  }

  int init(int argc, char** argv){
    if ( PlaneTrackerClientArgs::validFor(argc, argv) ) {
      auto client = createClient(argc, argv);
      sleepUnilExit();
    } else {
      reportInvalidArguments(argc, argv);  
      return -1;
    }
  }
}

int main(int argc, char** argv) {
  return PlaneTracking::init(argc, argv);  
}
