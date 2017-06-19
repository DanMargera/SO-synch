#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include <iostream>

// Error codes
static const int UNABLE_TO_CREATE_THREAD = 0,
                 UNABLE_TO_CREATE_MUTEX = 1,
                 UNABLE_TO_CREATE_SEMAPHORE = 2;

class ErrorHandler {
public:
  static void Error(int code) {
    std::cout << GetLastError() << '\n';
    switch(code) {
      case UNABLE_TO_CREATE_MUTEX:
      std::cout << "Error creating mutex!" << '\n';
      break;
      case UNABLE_TO_CREATE_SEMAPHORE:
      std::cout << "Error creating semaphore!" << '\n';
      break;
      case UNABLE_TO_CREATE_THREAD:
      std::cout << "Error creating thread!" << '\n';
      break;
    }
  }
private:
  ErrorHandler(){};
};

#endif
