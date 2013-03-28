#pragma once

namespace Theron {
  class Address;
}

namespace PlaneTracking {
  namespace Messages {
    struct Message; 
  }

  /**
   * All of the raw data receivers must extend this class. It guarantees some functionality.
   */
  class RawDataReceiver{
  public:
    virtual void start() = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
  protected:
    const Theron::Address targetAddress;
    void sendMessage(Message message);
  }
}
