#include "src/arguments.h"
#include <boost/none.h>

using namespace std;
using namespace boost;
using namespace boost::program_options;

namespace PlaneTracking{

  Arguments::Arguments(int argc, char** argv): argc(argc), argv(argv) {}
  Arguments::Arguments(): argc(0), argv(nullptr) {}

  variables_map Arguments::getArgumentMap() {
    const auto parsedOptions = command_line_parser(argc, argv)
                                .options(getOptions())
                                .allow_unregistered()
                                .run();

    variables_map vm;
    store(parsedOptions, vm);
    notify(vm);
    return vm;
  }

  bool Arguments::validFor(int argc, char** argv) const {
    return invalidOptions(argc, argv).empty();
  }

  bool Arguments::invalidOptions(int argc, char** argv) const {
    const auto parsed = command_line_parser(argc,argv)
                          .options(getOptions())
                          .allow_unregistered()
                          .run();
    return collect_unrecognized(parsedOptions.options, include_positional);
  }

  /**
   * Plane Tracker Client Arguments
   */
  PlaneTrackerClientArgs::PlaneTrackerClientArgs(int argc, char** argv) : Arguments(),
    UIArgs(argc, argv), PlaneTrackerBackendArgs(argc, argv){}

  options_description PlaneTrackerClientArgs::getDescription() const {
    options_description description("Plane Tracker Client Arguments");
    description.add(uiArgs.getDescription()).add(planeTrackerBackendArgs.getDescription());
    return description;
  }

  /**
   * UI Arguments 
   */
  UIArgs::UIArgs(int argc, char** argv) : Arguments(argc, argv) {
    auto map = getArgumentMap();
    videoScale = VideoScale(map["scale"].as<double>());
  }

  options_description UIArgs::getDescription const {
    options_description description("UI Arguments");
    description.add_options("scale",
                            value<double>(1.0)->default_value(1.0),
                            "Video Display Scale");
    return description;
  }

  PlaneTrackerBackendArgs::PlaneTrackerBackendArgs(int argc, char** argv):
    Arguments(argc, argv),
    GPSArgs(argc, argv),
    VideoArgs(argc, argv),
    CommandArgs(argc, argv) {}

  options_description PlaneTrackerBackendArgs::getDescription const {
    options_description description("Plane Tracker Backend Arguments");
    description.add(GPSArgs::getDescription())
               .add(VideoArgs::getDescription())
               .add(CommandArgs::getDescription());

    return description;
  }

  GPSArgs::GPSArgs(int argc, char** argv): Arguments(argc, argv) {
    auto map = getArgumentMap();
    planeSerialConfig = getPlaneSerialConfig(map);
    trackerSerialConfig = getTrackerSerialConfig(map);
    trackerPosition = getTrackerPosition(map);
  }

  optional<SerialConnectionData> GPSArgs::getPlaneSerialConfig(variables_map map) const {
    return getSerialConfig("plane-gps-port", "plane-gps-baud", map);
  }

  optional<SerialConnectionData> GPSArgs::getTrackerSerialConfig(variables_map map) const {
    return getSerialConfig("tracker-gps-port", "tracker-gps-baud", map);
  }

  optional<GPSCoordinate> GPSArgs::getTrackerPosition(variables_map map) {
    if (!map.count("alt") || !map.count("lat") || !map.count("lon")) {
      return optional<GPSCoordinate>(none);
    } else{
      const double latDbl = map["lat"].as<double>();
      const double lonDbl = map["lon"].as<double>();
      const double altDbl = map["alt"].as<double>();
      //TODO ? As(Degrees/Meters)
      const auto Degrees lat(latDbl);
      const auto Degrees lon(lonDbl);
      const auto Meters alt(altDbl);
      return optional<GPSCoordinate>(GPSCoordinate(lat, lon, alt));
    }
  }

  optional<SerialConnectionData> GPSArgs::getSerialConfig(string portKey,
                                                          string baudrateKey,
                                                          variables_map map) const {
    if (!map.count(portKey) || !map.count(baudrateKey)){
      return optional<SerialConnectionData>(none);
    } else {
      const string port = map[portKey].as<string>();
      const string baudrateStr = map[baudrateKey].as<string>();
      Baudrate baudrate(baudrateStr);
      return optional<SerialConnectionData>(SerialConnectionData(port, baudrate));
    }
  }

  options_description GPSArgs::getOptions() const {
    options_description description("GPS Options");
  }

  VideoArgs::VideoArgs(int argc, char** argv): Arguments(argc, argv) {
    auto map = getArgumentMap();
  }

  options_description VideoArgs::getDescription() const {
    options_description description("Vision Arguments");
    description.add_options("extras", "Display intermediate steps")
                           ("line", "Draw a line to the plane")
                           ("image", po::value<string>("")->default_value(""),
                            "Analyze a single image")
                           ("video", po::value<string>("")->default_value(""),
                            "Simulate using video file")
    return description;
  }

}
