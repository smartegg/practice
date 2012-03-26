#ifndef REF_COUNTED_OBJECT
#define REF_COUNTED_OBJECT
//
//@brief a basic class for objects that   employ reference counting
//based GC.
//forbid construct by copy and assignment.

class RefCountedObject {
  public:
   RefCountedObject();

   void duplicate() const;
   void release() const;
   int referenceCount() const;
  protected:
   virtual ~RefCountedObject();
  private:
   mutable int counter_;

   RefCountedObject(const RefCountedObject&);
   RefCountedObject& operator=(const RefCountedObject&);
};

inline int RefCountedObject::referenceCount() const {
  return counter_;
}

inline void RefCountedObject::duplicate() const {
  ++counter_;
}

inline void RefCountedObject::release() const {
  if (--counter_ == 0)
    delete this;
}
#endif
