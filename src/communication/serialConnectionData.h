#pragma once

namespace PlaneTracking{

  struct Baudrate {
    Baudrate(std::string baudrate)
    int baudrate;
  }

  /**
   * This struct represent parameters for a serial connection
   */
  struct SerialConnectionData{
    std::string port;
    Baudrate baudrate;
  };

}
