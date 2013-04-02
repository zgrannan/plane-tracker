namespace PlaneTracking {
  namespace Units {
    template <typename T>
    struct Scalar<T> {
      T value; 
    }

    struct Meters : Scalar<double>;
    struct Degrees : Scalar<double>;
    struct Pixels: : Scalar<unsigned int>;
    struct MilliSeconds : Scalar<unsigned int>;
    struct VideoScale : Scalar<unsigned double>
  }
}
