// using standard exceptions
#include <iostream>
//#include <exception>
#include <string>

using namespace std;

class myexception//: public exception
{
  public:
  //virtual const char* what() const throw()
  int error_numb;
  std::string what() throw()
  {
    if(error_numb == 1)
    {
        return "My exception happened";
    }
    
    else return "stupid";
  }
} myex;

int main () {
  try
  {
    myex.error_numb = 1;
    throw myex;
  }
  //catch (exception& e)
  catch (myexception& e)
  {
    cout << e.what() << '\n';
  }
  return 0;
}