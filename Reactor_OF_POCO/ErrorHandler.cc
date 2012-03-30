#include "ErrorHandler.h"

#include "SingletonHolder.h"
#include <iostream>

ErrorHandler* ErrorHandler::pHandler_ = 
        ErrorHandler::defaultHandler();

ErrorHandler::ErrorHandler() {
  
}

ErrorHandler::~ErrorHandler() {

}

void ErrorHandler::exception(const Exception& exc) {
  std::cerr << exc.displayText() << std::endl;
}

void ErrorHandler::exception(const std::exception& exc) {
  std::cerr << exc.what() << std::endl;
}

void ErrorHandler::exception() {
  std::cerr << "unknown exception" << std::endl;
}

void ErrorHandler::handle(const Exception& exc) {
  try {
    pHandler_->exception(exc);
  }
  catch (...) {

  }
}

void ErrorHandler::handle(const std::exception& exc) {
  try {
    pHandler_->exception(exc);
  }
  catch(...) {

  }
}

void ErrorHandler::handle() {
  try {
    pHandler_->exception();
  }
  catch(...)  {

  }
}

ErrorHandler* ErrorHandler::set(ErrorHandler* pHandler) {
  ErrorHandler* pOld = pHandler_;
  pHandler_ = pHandler;
  return pOld;
}

ErrorHandler* ErrorHandler::defaultHandler() {
  static SingletonHolder<ErrorHandler> sh;
  return sh.get();
}
