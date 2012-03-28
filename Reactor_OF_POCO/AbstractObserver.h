#ifndef ABSTRACT_OBSERVER_H
#define ABSTRACT_OBSERVER_H

#include "Notification.h"

//the basic class of all instantiations of 
//the Observer and NObserver template class
class AbstractObserver {
 public:
  AbstractObserver();
  AbstractObserver(const AbstractObserver&);
  virtual ~AbstractObserver();

  AbstractObserver& operator= (const AbstractObserver& observer);

  virtual void notify(Notification* pNf) const = 0;
  virtual bool equals(const AbstractObserver& observer) const = 0;
  virtual bool accepts(Notification* pNf) const = 0;
  virtual AbstractObserver* clone() const = 0;
  virtual void disable() = 0;

};
#endif
