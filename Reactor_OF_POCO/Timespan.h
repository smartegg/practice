#ifndef TIMESPAN_H
#define TIMESPAN_H

#include "Timestamp.h"

//@brief a class that represents time spans in microseconds resolution.
class Timespan {
 public:
  typedef Timestamp::TimeDiff  TimeDiff;

  Timespan();

  Timespan(TimeDiff microseconds);
  Timespan(long seconds, long microseconds);

  Timespan(int days, int hours, int minutes,
           int seconds, int microseconds);

  Timespan(const Timespan& timespan);

  ~Timespan();

  Timespan& operator= (const Timespan& timespan);

  Timespan& operator= (TimeDiff microseconds);

  Timespan& assign(int days, int hours, int minutes, int seconds,
                   int microseconds);
  
  Timespan& assing(long seconds, long microseconds);

  void swap(Timespan& timespan);

  bool operator== (const Timespan& timespan) const;
  bool operator!= (const Timespan& timespan) const;
  bool operator>  (const Timespan& timespan) const;
  bool operator>= (const Timespan& timespan) const;
  bool operator<  (const Timespan& timespan) const;
  bool operator<= (const Timespan& timespan) const;

  bool operator== (TimeDiff microseconds) const;
  bool operator!= (TimeDiff microseconds) const;
  bool operator>  (TimeDiff microseconds) const;
  bool operator>= (TimeDiff microseconds) const;
  bool operator<  (TimeDiff microseconds) const;
  bool operator<= (TimeDiff microseconds) const;

  Timespan operator + (const Timespan& d) const;
  Timespan operator - (const Timespan& d) const;
  Timespan& operator += (const Timespan& d);
  Timespan& operator -= (const Timespan& d);

  Timespan operator + (TimeDiff microseconds) const;
  Timespan operator - (TimeDiff microseconds) const;
  Timespan& operator += (TimeDiff microseconds);
  Timespan& operator -= (TimeDiff microseconds);

  int days() const;
  int hours() const;
  int totalHours() const;
  int minutes() const;
  int totalMinutes() const;
  int seconds() const;
  int totalSeconds() const;
  int milliseconds() const;
  TimeDiff totalMilliseconds() const;
  int microseconds() const;
  TimeDiff totalMicroseconds() const;
  int useconds() const;

  static const TimeDiff MILLISECONDS;
  static const TimeDiff SECONDS;
  static const TimeDiff MINUTES;
  static const TimeDiff HOURS;
  static const TimeDiff DAYS;
 private:
  TimeDiff span_;
};

inline int Timespan::days() const {
  return int(span_/DAYS);
}

inline int Timespan::hours() const {
  return int((span_/HOURS) % 24);
}

inline int Timespan::totalHours() const {
  return int(span_/HOURS);
}

inline int Timespan::minutes() const {
  return int((span_/MINUTES) % 60);
}

inline int Timespan::totalMinutes() const {
  return int(span_/MINUTES);
}

inline int Timespan::seconds() const {
  return int((span_/SECONDS)%60);
}

inline int Timespan::totalSeconds() const {
  return int(span_/SECONDS);
}

inline int Timespan::milliseconds() const {
  return int((span_/MINUTES)%1000);
}

inline Timespan::TimeDiff Timespan::totalMilliseconds() const {
  return TimeDiff(span_/MILLISECONDS);
}

inline int Timespan::microseconds() const{
  return int(span_ % 1000);
}

inline int Timespan::useconds() const  {
  return int(span_ % 1000000);
}

inline Timespan::TimeDiff Timespan::totalMicroseconds() const {
  return span_;
}

inline bool Timespan::operator== (const Timespan& timespan) const {
  return span_ == timespan.span_;
}


inline bool Timespan::operator!= (const Timespan& timespan) const {
  return span_ != timespan.span_;
}


inline bool Timespan::operator< (const Timespan& timespan) const {
  return span_ < timespan.span_;
}

inline bool Timespan::operator<= (const Timespan& timespan) const {
  return span_ <= timespan.span_;
}

inline bool Timespan::operator> (const Timespan& timespan) const {
  return span_ > timespan.span_;
}

inline bool Timespan::operator>= (const Timespan& timespan) const {
  return span_ >= timespan.span_;
}

inline bool Timespan::operator== (TimeDiff microseconds) const {
  return span_ == microseconds;
}

inline bool Timespan::operator!= (TimeDiff microseconds) const {
  return span_ != microseconds;
}

inline bool Timespan::operator< (TimeDiff microseconds) const {
  return span_ < microseconds;
}

inline bool Timespan::operator<= (TimeDiff microseconds) const {
  return span_ <= microseconds;
}

inline bool Timespan::operator> (TimeDiff microseconds) const {
  return span_ > microseconds;
}

inline bool Timespan::operator>= (TimeDiff microseconds) const {
  return span_ >= microseconds;
}

inline void swap(Timespan& s1, Timespan& s2) {
  s1.swap(s2);
}
#endif
