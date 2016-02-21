#ifndef WINDOW_HPP
#define WINDOW_HPP
#include "../error.hpp"

/* Abstract window class to open and
 * handle a window.
 */
class Window {
  public:
    virtual ~Window() = 0;

    virtual void setResolution(int width, int height) = 0;
    virtual void getResolution(int &width, int &height) = 0;
    virtual void setTitle(const std::string title) = 0;
    virtual void display() = 0;
};

/* Class to display error messages to the
 * user in a friendly manner.
 * Error window must be able to be created
 * without throwing an exception.
 */
class ErrorWindow {
  public:
    virtual ~ErrorWindow() = 0;

    virtual void displayError(Error &error) = 0;
};

#endif
