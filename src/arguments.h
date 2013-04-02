#pragma once

namespace PlaneTracking {

  struct Arguments {
    Arguments(int argc, char** argv);
    Arguments();
    static virtual bool validFor(int argc, char** argv) const;
    static virtual std::vector<std::string>> invalidOptions(int argc, char** argv) const;
  protected:
    static virtual boost::program_options::options_description getOptions() const = 0;
    virtual boost::program_options::variables_map getArgumentMap();
  private:
    const int argc;
    const char **argv;
  };

  struct PlaneTrackerClientArgs : Arguments {
    PlaneTrackerClientArgs(int argc, char** argv);
    const UIArgs uiArgs;
    const PlaneTrackerBackendArgs backendArgs;
  protected:
    static virtual boost::program_options::options_description getDescription() const override;
  };

  struct UIArgs : Arguments{
    UIArgs(int argc, char** argv);
    VideoScale scale;
  protected:
    static virtual boost::program_options::options_description getDescription() const override;
  };

  struct PlaneTrackerBackendArgs : Arguments {
    PlaneTrackerBackendArgs(int argc, char** argv);
    GPSArgs gpsArgs;
    VideoArgs videoArgs;
    CommandArgs gimbalArgs;

  protected:
    static virtual boost::program_options::options_description getDescription() const override;
  };

  struct GPSArgs : Arguments {
    GPSArgs(int argc, char** argv);

    boost::optional<SerialConnectionData> planeSerialConfig;
    boost::optional<SerialConnectionData> trackerSerialConfig;
    boost::optional<GPSCoordinate> trackerPosition;
  protected:
    static virtual boost::program_options::options_description getDescription() const override;

  private:
    boost::optional<SerialConnectionData> getSerialConfig(std::string portKey,
                                                          std::string baudrateKey,
                                                          boost::program_options::variables_map)
      const;

    boost::optional<SerialConnectionData> getPlaneSerialConfig(boost::program_options::variables_map) const;
    boost::optional<SerialConnectionData> getTrackerSerialConfig(boost::program_options::variables_map) const;
    boost::optional<GPSCoordinate> getTrackerPosition(boost::program_options::variables_map) const;
  };

  struct VideoArgs : Arguments {
    VideoArgs(int argc, char** argv);
  protected:
    static virtual boost::program_options::options_description getDescription() const override;
  };

  struct CommandArgs : Arguments {
    CommandArgs(int argc, char** argv);
  protected:
    static virtual boost::program_options::options_description getDescription() const override;
  };
}
