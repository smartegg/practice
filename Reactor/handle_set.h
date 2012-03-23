#ifndef HANDLE_SET_H
#define HANDLE_SET_H
#include <sys/select.h>
#include "event_handler.h"

#define howmany(x,y) (((x) + (y) -1)/(y))
//@brief a wrapper of fd_set
class HandleSet {
 public:
  enum {
    kMaxSize = FD_SETSIZE
  };
  HandleSet();
  HandleSet(const fd_set& mask);
  //clear bitmasks to all 0's.
  void Reset();
  //check whether handle is enabled.
  int IsSet(Handle handle)const;
  //enable the @a handle.
  void SetBit(Handle handle);
  //disable @a handle
  void ClrBit(Handle handle);
  //return the number of the large bits.
  Handle MaxSet()const;
  //return the number of the enables bits.
  int NumSet()const;
  //updaete the max_handle and size info.
  void Sync(Handle max_handle);
  //?? whatis this ?
  operator fd_set*();
  fd_set* fdset();
 private:
  int size_;
  Handle max_handle_;
  fd_set mask_;
  enum {
    kWordSize = NFDBITS,
    kNbits = 256,
    kNumWords = howmany(kMaxSize,NFDBITS)
  };

  static int CountBits(long n);
  //update the <max_handle_>
  void SetMax(Handle max);
  
};
#endif //HANDLE_SET
