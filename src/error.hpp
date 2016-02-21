#ifndef ERROR_HPP
#define ERROR_HPP
#include <string>

/* Class to represent error messages created by the engine.
 */
class Error {
  public:
    Error(const std::string &type, const std::string &message);

    std::string &getType();
    std::string &getMessage();
  private:
    std::string type, message;
};
#endif
