#include "Exception.h"
#include <typeinfo>


Exception::Exception(int code) : pNested_(0), code_(code) {

}

Exception::Exception(const std::string& msg, int code) : msg_(msg),
                                                        pNested_(0),
                                                        code_(code) {
}

Exception::Exception(const std::string& msg,
                     const std::string& arg,
                     int code) : msg_(msg),
                                 pNested_(0),
                                 code_(code) {
  if (!arg.empty()) {
    msg_.append(": ");
    msg_.append(arg);
  }
}

Exception::Exception(const std::string& msg,
                     const Exception& nested,
                     int code) : msg_(msg),
                                pNested_(nested.clone()),
                                code_(code) {

}

Exception::Exception(const Exception& exc) : std::exception(exc),
                                             msg_(exc.msg_),
                                             code_(exc.code_){
  pNested_ = exc.pNested_? exc.pNested_->clone() : 0;
}

Exception::~Exception() throw (){
  delete pNested_;
}

Exception& Exception::operator= (const Exception& exc) {
  if (&exc != this) {
    delete pNested_;
    msg_ = exc.msg_;
    code_ = exc.code_;
    pNested_ = exc.pNested_? exc.pNested_->clone() : 0;
  }

  return *this;
}

const char* Exception::name() const throw() {
  return "Exception";
}

const char* Exception::className() const throw() {
  return typeid(*this).name();
}

const char* Exception::what() const throw(){
  return name();
}

std::string Exception::displayText() const {
  std::string txt = name();
  if (!msg_.empty()) {
    txt.append(": ");
    txt.append(msg_);
  }
  return txt;
}

void Exception::extenedMessage(const std::string& arg) {
  if (!arg.empty()) {
    if (!msg_.empty()) 
      msg_.append(": ");
    msg_.append(arg);
  }
}

Exception* Exception::clone() const {
  return new Exception(*this);
}

void Exception::rethrow() const {
  throw *this;
}

IMPLEMENT_EXCEPTION(LogicException, Exception, "Logic exception");
IMPLEMENT_EXCEPTION(NullPointerException, LogicException,
                    "null value");
IMPLEMENT_EXCEPTION(InvalidArgumentException, LogicException,
                    "Invalid argument");
IMPLEMENT_EXCEPTION(NotImplementedException, LogicException,
                    "not implemented");

IMPLEMENT_EXCEPTION(RuntimeException, Exception, "Runtime exception ");
IMPLEMENT_EXCEPTION(SystemException, RuntimeException, "System exception");
