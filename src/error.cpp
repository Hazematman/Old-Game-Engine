#include "error.hpp"
using namespace std;

/* Simply initalize error class with error objects
 */
Error::Error(const string &type, const string &message) : 
  type(type), message(message) {}

string &Error::getType() {
  return type;
}

string &Error::getMessage() {
  return message;
}
