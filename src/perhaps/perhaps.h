#ifndef OPTION_H
#define OPTION_H
#include <vector>
#include <exception>
#include <functional>
using namespace std;

class noneGetException: public exception
{
  virtual const char* what() const throw() {
    return "Can't get a value from an empty option"; 
  }
};

template <typename T>
class Option{
  public:
    
    /**
     * Creates a new Option that contains a the given value
     */
    Option(T value): value(value) ,hasValue(true) {}

    /**
     * Creates a new Option that does not contain a value
     */
    Option(): hasValue(false) {}

    /**
     * Returns true if this option contains a value, false otherwise
     */
    bool isDefined() const { return hasValue; }

    /**
     * Returns true if this option does not contain a value, false otherwise
     */
    bool isEmpty() const { return !hasValue; }

    /**
     * Returns true if this option contains a value and applying the function p
     * to the value return true
     */
    bool exists( function<bool(T)> f) const;

    /**
     * Returns true if this option and the other option do not contain a value, or 
     * if they both contain the same value.
     */
    template<typename B>
    bool operator==(const Option<B> &other) const { 
      if (!(this->hasValue) && !(other.isDefined())) {
        return true;
      } else {
        return this->get() == other.get();
      }
    }
    
    /**
     * Returns true if this option and the other option contain different values,
     * or if one contains a value and the other does not
     */
    template<typename B>
    bool operator!=(const Option<B> &other) const { 
      if (this->isDefined() == false && other.isDefined() == false) {
        return false;
      } else if (this->isDefined() != other.isDefined()) {
        return true;
      } else {
        return this->get() != other.get();
      }
    }

    /**
     * Returns the value that this option contains. If the option does not contain
     * a value, a "noneGetException" is thrown
     */
    T operator*() const{ return this->get(); }

    /**
     * Returns a copy of this Option if it is nonempty and applying f to this 
     * Option's value returns true
     */
    Option<T> filter(function<bool(T)> f) const;

    /**
     * Returns a copy of this Option if it is nonempty and applying f to this 
     * Option's value returns false 
     */
    Option<T> filterNot(function<bool(T)> f) const;


    /**
     * Returns the value that this option contains. If the option does not contain
     * a value, a "noneGetException" is thrown
     */
    T get() const;

    /**
     * Returns the result of applying f to this option's value if this object has 
     * a value. Otherwise returns an option that does not contain a value
     */
    template<typename B>
    Option<B> flatMap(function<Option<B>(T)> f) const {
      if (!hasValue){
        return Option<B>();
      } else {
        return f(value);
      }
    }

    /**
     * Apply's the function f on this option's value if the option has a value
     */
    void foreach(function<void(T)> f) const;

    /**
     * Returns the result of applying f to this option's value if this object has 
     * a value, wrapping it in an Option. Otherwise returns an option that does not contain
     * a value
     */
    template<typename B>
    Option<B> map(function<B(T)> f) const {
      if (!hasValue) {
        return Option<B>();
      } else {
        return Option<B>(f(value));
      }
    }

    /**
     * If this option has a value, it returns that value. Otherwise it returns _else
     */
    T getOrElse (T _else) const;

    /**
     * If this option has a value, it returns this option. Otherwise it evaluates _else
     * and returns that option instead.
     */
     Option<T> orElse (function<Option<T>()> _else) const;

    /**
     * If this option has a value, this returns a vector containing that value.
     * Otherwise it returns an empty vector
     */
    vector<T> toVector() const;

  private:

    T value;
    bool hasValue;
};

template <typename T>
class None : public Option<T>{
  public:
    None(): Option<T>(){}
};

template <typename T>
class Some: public Option<T>{
  public:
    Some(T value): Option<T>(value){};
};

template <typename T>
bool Option<T>::exists(function<bool(T)> p) const {
  return hasValue && p(value);
}

template <typename T>
Option<T> Option<T>::filter(function<bool(T)> f) const {
  if (!hasValue || !f(value)) {
    return None<T>();
  } else {
    return Some<T>(value);
  }
}

template <typename T>
Option<T> Option<T>::filterNot(function<bool(T)> f) const {
  if (!hasValue || f(value)) {
    return None<T>();
  } else {
    return Some<T>(value);
  }
}

template <typename T>
T Option<T>::get() const {
  if (!hasValue){
    throw noneGetException();
  } else {
    return value;
  }
}

template <typename T>
void Option<T>::foreach(function<void(T)> f) const {
  if (hasValue) {
    f(value);
  }
}

template <typename T>
T Option<T>::getOrElse (T _else) const {
  if ( hasValue ) {
    return value;
  } else {
    return _else;
  }
}

template <typename T>
Option<T> Option<T>::orElse (function<Option<T>()> _else) const {
  if ( hasValue ) {
    return Option(value);
  } else {
    return _else();
  }
}

template <typename T>
vector<T> Option<T>::toVector() const {
  if (hasValue) {
    return vector<T>{value};
  } else{
    return vector<T>();
  }
}

#endif
