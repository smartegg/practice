#include "Timestamp.h"

#include "Exception.h"
#include <algorithm>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

Timestamp::Timestamp() {
  update();
}

Timestamp::Timestamp(TimeVal tv) {
  ts_ = tv;
}

Timestamp::Timestamp(const Timestamp& other) {
  ts_ = other.ts_;
}

Timestamp::~Timestamp() {

}

Timestamp& Timestamp::operator=(const Timestamp& other) {
  ts_ = other.ts_;
  return *this;
}

Timestamp& Timestamp::operator=(TimeVal tv) {
  ts_ = tv;
  return *this;
}

void Timestamp::swap(Timestamp& timestamp) {
  std::swap(ts_, timestamp.ts_);
}

Timestamp Timestamp::fromEpochTime(std::time_t t) {
  return Timestamp(TimeVal(t) * resolution());
}

Timestamp Timestamp::fromUtcTime(UtcTimeVal val) {
  val -= (TimeDiff(0x01b21dd2) << 32) + 0x13814000;                       
  val /=10;
  return Timestamp(val);
}

void Timestamp::update() {
  struct timeval tv;
  if (gettimeofday(&tv, NULL)) {
    throw SystemException("cannot get time of day");
  }
  ts_ = TimeVal((tv.tv_sec) * resolution() + tv.tv_usec);
}
