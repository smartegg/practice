#ifndef TIME_VALUE_H
#define TIME_VALUE_H

class TimeValue {
 public:
  Timevalue(long sec = 0, long usec = 0);
  TimeValue(const timeval& t);

  //Friend method
  friend TimeValue operator+ (const TimeValue& lhs,
                              const TimeValue& rhs);

  friend TimeValue operator- (const TimeValue& lhs,
                              const TimeValue& rhs);

  friend int operator< (const Timevalue& lhs,
                        const Timevalue& rhs);
 private:

};
#endif
