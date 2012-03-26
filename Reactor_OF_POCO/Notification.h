#ifndef NOTIFICATION_H
#define NOTIFICATION_H
#include "RefCountedObject.h"
#include "AutoPtr.h"

//@brief the base class for all notification classes.
class Notification : public RefCountedObject {
 public:
  typedef AutoPtr<Notification> Ptr;

  Notification();
  //return the name of the implementation
  //@default_implementation  return the className.
  virtual std::string name() const;
 protected:
  virtual ~Notification();

};
#endif
