#pragma once

namespace PlaneTracking{

  /**
   * This struct represent parameters for a serial connection
   */
  struct SerialConnectionData{
    std::string port;
    int baudrate;
    int options;
  };

}
