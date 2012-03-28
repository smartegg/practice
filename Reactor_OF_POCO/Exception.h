#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <stdexcept>
class Exception : public std::exception {
 public:
  Exception(const std::string& msg, int code = 0);
  Exception(const std::string& msg, const std::string& arg, int code = 0);
  Exception(const std::string& msg, const Exception& nested, int code = 0);
  Exception(const Exception& exc);
  Exception& operator= (const Exception& exc);
  ~Exception() throw();
  //return a static string describing the exception
  virtual const char* name() const throw();
  //return the name of the exceptionclass
  virtual const char* className() const throw();
  //same with name() , for compatibility with std::exception
  virtual const char* what() const throw();
  const Exception* nested() const ;
  const std::string& message() const;
  int code() const;
  //return the message name and text body.
  std::string displayText() const;
  virtual Exception* clone() const;
  virtual void rethrow() const;
  
 protected:
  Exception(int code = 0);
  void message(const std::string& msg);
  //append a string to the message body.
  void extenedMessage(const std::string& arg);

 private:
  std::string msg_;
  Exception* pNested_;
  int code_;
};

//inlines
inline const Exception* Exception::nested() const {
  return pNested_;
}

inline const std::string& Exception::message() const {
  return msg_;
}

inline void Exception::message(const std::string& msg) {
  msg_ = msg;
}

inline int Exception::code() const {
  return code_;
}


//Macors for quickly declaring and implementing exception classes.
#define DECLARE_EXCEPTION_CODE(CLS, BASE, CODE) \
class CLS : public BASE  {  \
 public:  \
    CLS(int code = CODE); \
    CLS(const std::string& msg, int code = CODE); \
    CLS(const std::string& msg, const std::string& arg, int code = CODE); \
    CLS(const std::string& msg, const Exception& exc, int code = CODE); \
    CLS(const CLS& exc); \
    ~CLS() throw(); \
    CLS& operator= (const CLS&); \
    const char* name()const throw(); \
    const char* className() const throw(); \
    Exception* clone() const; \
    void rethrow() const; \
};

#define DECLARE_EXCEPTION(CLS, BASE) DECLARE_EXCEPTION_CODE(CLS, BASE, 0)
#define IMPLEMENT_EXCEPTION(CLS, BASE, NAME)  \
  CLS::CLS(int code) : BASE(code) { \
  }   \
  CLS::CLS(const std::string& msg, int code) : BASE(msg, code) { \
  } \
  CLS::CLS(const std::string& msg, const std::string& arg, int code) \
    : BASE(msg, arg, code) { \
  }\
  CLS::CLS(const std::string& msg, const Exception& exc, int code)  \
    : BASE(msg, exc, code) { \
  } \
  CLS::CLS(const CLS& exc) : BASE(exc){ \
  } \
  CLS::~CLS() throw() { \
  } \
  CLS& CLS::operator= (const CLS& exc) { \
    BASE::operator=(exc); \
    return *this; \
  } \
  const char* CLS::name() const throw(){ \
    return NAME; \
  } \
  const char* CLS::className() const throw() { \
    return typeid(*this).name(); \
  } \
  Exception* CLS::clone() const { \
    return new CLS(*this); \
  }\
  void CLS::rethrow() const { \
    throw *this; \
  } 

DECLARE_EXCEPTION(LogicException, Exception);
DECLARE_EXCEPTION(NullPointerException, LogicException);
DECLARE_EXCEPTION(InvalidArgumentException, LogicException);
DECLARE_EXCEPTION(NotImplementedException, LogicException);


DECLARE_EXCEPTION(RuntimeException, Exception);
DECLARE_EXCEPTION(SystemException, RuntimeException);
DECLARE_EXCEPTION(TimeoutException, RuntimeException);
DECLARE_EXCEPTION(IOException, RuntimeException);


DECLARE_EXCEPTION(NetException, IOException);
DECLARE_EXCEPTION(InvalidAddressException, NetException);
DECLARE_EXCEPTION(InvalidSocketException, NetException);
DECLARE_EXCEPTION(ConnectionAbortedException, NetException);
DECLARE_EXCEPTION(ConnectionResetException, NetException);
DECLARE_EXCEPTION(ConnectionRefusedException, NetException);

#endif
