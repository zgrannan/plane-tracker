#pragma once
namespace PlaneTracker {
  struct GimbalCommand;
  class GimbalCommandProtocol : SerialWriteProtocol<GimbalCommand> {};
}
