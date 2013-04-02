namespace PlaneTracker {

  template typename<T>
  class SerialReadProtocol<T> {
    public:
      virtual boost::optional<T> fromString(string value);
  };

  template typename<T>
  class SerialWriteProtocol<T> {
    public:
      virtual string toString(T data);
  };

}
