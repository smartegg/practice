#ifndef RUNNABLE_H
#define RUNNABLE_H

//@brief this interface with the run() method must be implemented 
//by classes that provide an entry point for a thread.
class Runnable {
 public:
  Runnable();
  virtual ~Runnable();

  //do whatever the thread need to do.
  virtual void run() = 0;
};
#endif
