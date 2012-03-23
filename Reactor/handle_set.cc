#include "handle_set.h"
#include "handle_set.inl"
#include <cstring>

#define MSB_MASK (~((fd_mask) 1 << (NFDBITS - 1)))
#define DIV_BY_WORDSIZE(x) ((x) / (int)HandleSet::kWordSize)
#define MULT_BY_WORDSIZE(x) ((x) * HandleSet::kWordSize)

HandleSet::HandleSet() {
  this->Reset();
}

HandleSet::HandleSet(const fd_set& fd_mask) {
  this->Reset();
  memcpy(&this->mask_, &fd_mask, sizeof fd_mask);
  this->Sync(HandleSet::kMaxSize);
}

int HandleSet::CountBits(long n) {
  int rval = 0;
  for (long m = n; m != 0; m &= m -1)
    rval++;
  return rval;
}

void HandleSet::Sync(Handle max_handle) {
  fd_mask* maskp = (fd_mask*)(this->mask_.fds_bits);
  this->size_ = 0;

  for (int i = DIV_BY_WORDSIZE(max_handle - 1); i>=0; i--)
    this->size_ += HandleSet::CountBits(maskp[i]);
  this->SetMax(max_handle);
}

void HandleSet::SetMax(Handle max_handle) {
  fd_mask* maskp = (fd_mask*) (this->mask_.fds_bits);

  if (this->size_ == 0)
    this->max_handle_ = -1;
  else {
    int i;
    for (i = DIV_BY_WORDSIZE(max_handle - 1);maskp[i] == 0; i--)
      continue;
    this->max_handle_ = MULT_BY_WORDSIZE(i);
    for (fd_mask val = maskp[i]; (val & MSB_MASK) != 0;val <<= 1)
      ++this->max_handle_;
  }

  if (this->max_handle_ >= HandleSet::kMaxSize)
    this->max_handle_ = HandleSet::kMaxSize - 1;
}
