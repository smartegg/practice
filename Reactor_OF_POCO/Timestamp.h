#ifndef TIMESTAMP_H
#define TIMESTAMP_H
#include <ctime>
#include <stdint.h>

//@brief store a time value with microseconds resolution.
//can be compared and support simple arithmetics
//timestamps are UTC based.
class Timestamp {
 public:
  typedef int64_t TimeVal;
  typedef int64_t UtcTimeVal;
  typedef int64_t TimeDiff;

  Timestamp();
  Timestamp(TimeVal tv);
  Timestamp(const Timestamp& other);

  ~Timestamp();

  Timestamp& operator= (const Timestamp& other);
  Timestamp& operator= (TimeVal tv);

  void swap(Timestamp& timestamp);

  //update the timestamp with the current time using gettimeofday()
  void update();

  bool operator == (const Timestamp& ts) const;
  bool operator != (const Timestamp& ts) const;
  bool operator >  (const Timestamp& ts) const;
  bool operator >= (const Timestamp& ts) const;
  bool operator <  (const Timestamp& ts) const;
  bool operator <= (const Timestamp& ts) const;

  Timestamp operator + (TimeDiff d) const;
  Timestamp operator - (TimeDiff d) const;
  TimeDiff  operator - (const Timestamp& ts) const;
  Timestamp& operator += (TimeDiff d) ;
  Timestamp& operator -= (TimeDiff d);
  //resolution is one second
  std::time_t epochTime() const;
  //resolution is 100 nanoseconds
  UtcTimeVal utcTime() const;
  //resolution is one microsecond
  TimeVal epochMicroseconds() const;
  //return the time passed 
  //Equivalent to Timestamp() - *this;
  TimeDiff elapsed() const;
  //check whether time passed from this timestamp >= interval
  bool isElapsed(TimeDiff interval) const;

  static Timestamp fromEpochTime(std::time_t t);
  static Timestamp fromUtcTime(UtcTimeVal t);

  //return the resolution in units per second.
  //because the resolutin here is microsecond.
  //always return 1000000
  static TimeVal resolution();

 private:
  TimeVal ts_;
};

inline bool Timestamp::operator== (const Timestamp& ts) const {
  return ts_ == ts.ts_;
}

inline bool Timestamp::operator!= (const Timestamp& ts) const {
  return ts_ != ts.ts_;
}

inline bool Timestamp::operator>  (const Timestamp& ts) const {
  return ts_ > ts.ts_;
}

inline bool Timestamp::operator>= (const Timestamp& ts) const {
  return ts_ >= ts.ts_;
}

inline bool Timestamp::operator<  (const Timestamp& ts) const {
  return ts_ < ts.ts_;
}

inline bool Timestamp::operator<= (const Timestamp& ts) const {
  return ts < ts.ts_;
}


inline Timestamp Timestamp::operator+ (TimeDiff d) const {
  return Timestamp(ts_ + d);
}

inline Timestamp Timestamp::operator- (TimeDiff d) const {
  return Timestamp(ts_ - d);
}

inline Timestamp::TimeDiff Timestamp::operator- (const Timestamp& ts) const {
  return  ts_ - ts.ts_;
}

inline Timestamp& Timestamp::operator+= (TimeDiff d) {
  ts_ += d;
  return *this;
}

inline Timestamp& Timestamp::operator-= (TimeDiff d) {
  ts_ -= d;
  return *this;
}

inline std::time_t Timestamp::epochTime() const {
  return std::time_t(ts_/resolution());
}

inline Timestamp::UtcTimeVal Timestamp::utcTime() const {
  return ts_ * 10 + (TimeDiff(0x01b21dd2) << 32) + 0x13814000;
}

inline Timestamp::TimeVal Timestamp::epochMicroseconds() const {
  return ts_;
}

inline Timestamp::TimeDiff Timestamp::elapsed() const {
  Timestamp now;
  return now - *this;
}

inline bool Timestamp::isElapsed(Timestamp::TimeDiff interval) const {
  Timestamp now;
  Timestamp::TimeDiff diff =  now - *this;
  return diff >= interval;
}

inline Timestamp::TimeVal Timestamp::resolution() {
  return 1000000;
}

inline void swap(Timestamp& s1, Timestamp& s2) {
  s1.swap(s2);
}


#endif
