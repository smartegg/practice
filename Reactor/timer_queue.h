#ifndef TIMER_QUEUE_H
#define TIMER_QUEUE_h

class Timevalue;


class TimerQueue {
 public:
  //return true if queue is empty
  int IsEmpty(void) const ;
  //return earliest time in queue
  const TimeValue& EarliestTime()const;

  //schedule a <handler> to be dispatched at the <future time>
  //and at subsequent <interval>s.
  //return a <timer_id>  that can be used to cancel the timer
  virtual int Schedule(EventHandler* handler,
                        const void* act,
                        const TimeValue& future_time,
                        const TimeValue& interval);

  //cancel all <handler> matches(which can be registered many times)
  virtual int Cancel(EventHandler* handler);

  //cancel the single eventhandle by <timer_id> returned from Schedule
  virtual int Cancel(int timer_id, const void** act = 0);

  //expire all timers <= <expire_time>
  virtual void expire(const TimeValue& expire_time);
 private:
};
#endif
