#include "perhaps.h"
#include <iostream>
#include <cassert>
using namespace std;

void throwException(int dummy){
  throw exception();
}

Some<int> someIntFunc(){
  return Some<int>(2);
}

None<int> noneIntFunc(){
  return None<int>();
}

Option<double> safeDivide (int a, int b){
  if ( b == 0 ) {
    return None<double>();
  } else{
    return Some<double>((double)a/(double)b);
  }
}

double toDouble (int input){
  return (double)input;
}

bool isEven(int x){
  return x % 2 == 0;
}

int main(int argc, char* argv[]) {
  cout << "Testing options...\n";
  
  assert( safeDivide(4,2) == Some<double>(2.0) );
  assert( safeDivide(5,0) == None<double>() );

  cout << "Tested basic functionality\n";

  const Option<int> fiveOption = Some<int>(5);
  const Option<int> sixOption = Some<int>(6);
  const Option<int> noneOption = None<int>();

  assert( fiveOption.get() == 5 );
  assert( *fiveOption == 5 );
  try {
    noneOption.get();
    assert(false);
  } catch (noneGetException){};

  cout <<"Tested 'get'\n";

  assert( noneOption.isEmpty() );
  assert( !fiveOption.isEmpty() );
  cout << "Testing isEmpty\n";

  assert( !fiveOption.exists(isEven) );
  assert( !noneOption.exists(isEven) );
  assert( sixOption.exists(isEven) );
  cout << "Tested exists\n";
  
  assert( fiveOption.filter(isEven) == None<int>() );
  assert( sixOption.filter(isEven) == Some<int>(6) );
  assert( noneOption.filter(isEven) == None<int>() );
  cout << "Tested filter\n";

  assert( fiveOption.filterNot(isEven) == Some<int>(5) );
  assert( sixOption.filterNot(isEven) == None<int>() );
  assert( noneOption.filterNot(isEven) == None<int>() );
  cout << "Tested filterNot\n";

  function<Option<double>(int)> div5 = [](int num){
    return safeDivide(num,5);
  };

  function<Option<double>(int)> div0 = [](int num){
    return safeDivide(num,0);
  };

  assert( fiveOption.flatMap(div5) == Some<double>(1) );
  assert( fiveOption.flatMap(div0) == None<double>() );
  assert( noneOption.flatMap(div0) == None<double>() );
  cout << "Tested flatMap\n";
  
  try{
    fiveOption.foreach(throwException);
    assert(false);
  } catch (exception){}

  noneOption.foreach(throwException);
  cout << "Tested foreach\n";

  assert( fiveOption.getOrElse(6) == 5 );
  assert( noneOption.getOrElse(6) == 6 );
  cout << "Tested getOrElse\n";

  assert( fiveOption.orElse(someIntFunc) == Some<int>(5) );
  assert( noneOption.orElse(someIntFunc) == Some<int>(2) );
  assert( noneOption.orElse(noneIntFunc) == None<int>() );
  cout << "Tested orElse\n";


  assert ( fiveOption.isDefined() );
  assert ( !noneOption.isDefined() );
  cout << "Tested isDefined\n";

  vector<int> fiveVector = fiveOption.toVector();
  vector<int> noneVector = noneOption.toVector();
  assert( fiveVector.size() == 1 && fiveVector[0] == 5 );
  assert( noneVector.size() == 0 );
  cout << "Tested toVector\n";
  
  function<double(int)> _toDouble = toDouble; 
  assert( fiveOption.map(_toDouble) == Some<double>(5.0) );
  assert( noneOption.map(_toDouble) == None<double>() );
  cout << "Tested map\n";

  assert( None<int>() == None<double>() );
  assert( Option<int>() == None<double>() );
  assert( Option<int>(5) == Option<int>(5) );
  assert( Option<int>(5) == Some<int>(5) );
  assert( Some<int>(5) == Some<int>(5) );
  assert( Some<int>(5) != Option<int>(6) );
  assert( Some<double>(4.0) != Some<int>(3) );
  assert( Option<int>(4) != Option<int>(5) ); 
  cout << "Tested equality\n";

  cout << "All tests passed successfully.\n";
  return 0;
}
