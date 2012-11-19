Perhaps: An Option Class for C++
--------------------------------
**About**

This is an implementation of Scala's Option class for C++. Implementing the Option
pattern is useful when creating functions that may not yield a valid result. Options provide
a simple way to return a "failure", instead of returning a null pointer (which may not be
possible) or throwing an exception (which interrupts control flow and leads to ugly code).
For example, consider this function that performs division using the Option class:

    Option<int> safeDivide (int a, int b){
      if ( b == 0 ) {
        return None<int>();
      } else{
        return Some<int>(a/b);
      }
    }

If the caller attempts to divide by zero, this function simply returns a None option instead
of throwing an exception. The caller is responsible for checking whether or not the option has a
value.

This Option class has also included most of the methods from Scala's Option class. Check out
[this page](http://www.scala-lang.org/api/current/scala/Option.html) for more cool stuff you
can do with Options.

**Usage**

Simply #include "perhaps.h" in order to use Options. Perhaps requires std::function, 
so you must enable c++11 features in your compiler to use Options.

When using `map` or `flatMap`, you may need to pass the function argument as a C++11
std::function instead of a C-style function pointer. It's pretty easy to create an 
std::function object from a function:

    double toDouble (int input){
      return (double)input;
    }
    function<double(int)> _toDouble = toDouble; 

Just like in Scala, these Options are immutable. However, it would be easy to modify the code
to expose the `value` member if you really wanted to.

**Examples**

Check out `test.cpp` for examples of how to use the Option class, or just look
at `perhaps.h`, as the code is pretty straightforward.
