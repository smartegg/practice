#include "Timespan.h"

#include <algorithm>

const Timespan::TimeDiff Timespan::MILLISECONDS = 1000;
const Timespan::TimeDiff Timespan::SECONDS = 
          1000 * Timespan::MILLISECONDS;
const Timespan::TimeDiff Timespan::MINUTES = 
          60 * Timespan::SECONDS;
const Timespan::TimeDiff Timespan::HOURS = 
          60 * Timespan::MINUTES;
const Timespan::TimeDiff Timespan::DAYS =
          24 * Timespan::HOURS; 

Timespan::Timespan() : span_(0) {

}

Timespan::Timespan(TimeDiff microseconds) : span_(microseconds) {

}

Timespan::Timespan(long seconds, long microseconds) : 
    span_(TimeDiff(seconds) * SECONDS + microseconds)  {

}

Timespan::Timespan(int days, int hours, int minutes, int seconds,
                   int microseconds) : 
  span_(TimeDiff(microseconds) + TimeDiff(seconds) * SECONDS +
        TimeDiff(microseconds) * MINUTES + TimeDiff(hours) * HOURS +
        TimeDiff(days) * DAYS) {

}

Timespan::Timespan(const Timespan& timespan) : span_(timespan.span_) {
  
}

Timespan::~Timespan() {

}

Timespan& Timespan::operator= (const Timespan& timespan) {
  span_ = timespan.span_;
  return *this;
}

Timespan&  Timespan::operator= (TimeDiff microseconds) {
  span_ = microseconds;
  return *this;
}

Timespan& Timespan::assign(int days, int hours, int minutes, int seconds,
                   int microseconds) {
  span_ = TimeDiff(microseconds) + TimeDiff(seconds) * SECONDS +
        TimeDiff(microseconds) * MINUTES + TimeDiff(hours) * HOURS +
        TimeDiff(days) * DAYS;
  return *this;
}

Timespan& Timespan::assing(long seconds, long microseconds) {
  span_ = TimeDiff(seconds) * SECONDS + TimeDiff(microseconds);
  return *this;
}

void Timespan::swap(Timespan& timespan) {
  std::swap(span_, timespan.span_);
}

Timespan Timespan::operator + (const Timespan& d) const {
  return Timespan(span_ + d.span_);
}

Timespan Timespan::operator - (const Timespan& d) const {
  return Timespan(span_ - d.span_);
}

Timespan& Timespan::operator += (const Timespan& d) {
  span_ += d.span_;
  return *this;
}

Timespan& Timespan::operator -= (const Timespan& d) {
  span_ -= d.span_;
  return *this;
}

Timespan Timespan::operator + (TimeDiff microseconds) const {
  return Timespan(span_ + microseconds);
}

Timespan Timespan::operator - (TimeDiff microseconds) const {
  return Timespan(span_ - microseconds);
}

Timespan& Timespan::operator += (TimeDiff microseconds) {
  span_ += microseconds;
  return *this;
}

Timespan& Timespan::operator -= (TimeDiff microseconds) {
  span_ -= microseconds;
  return *this;
}

 
