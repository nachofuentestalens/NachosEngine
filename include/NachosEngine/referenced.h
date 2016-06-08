// ----------------------------------------------------------------------------
// Copyright (C) 2014 Jose L. Hidalgo 
// Referenced Class.
// ----------------------------------------------------------------------------
#ifndef __REFERENCED_H__
#define __REFERENCED_H__

#include <cassert>

namespace NachosEngine {

  // Referenced base class
  class Referenced {
  public:
    unsigned int ref_counter() const { return ref_counter_; }
    void ref();
    void unref();
    void unref_noDelete();

  protected:
    Referenced() : ref_counter_(0) {}
    virtual ~Referenced() {}

  private:
    unsigned int ref_counter_;
    Referenced(const Referenced &);
    Referenced& operator=(const Referenced &);
  };

  // Inline Implementations -------------------------------------------------

  inline void Referenced::ref() {
    ++ref_counter_;
  }

  inline void Referenced::unref() {
    assert(ref_counter_ > 0);
    --ref_counter_;
    if (ref_counter_ == 0) {
      delete this;
    }
  }
  
  inline void Referenced::unref_noDelete() {
    assert(ref_counter_ > 0);
    --ref_counter_;
  }

}  // NachosEngine

#endif  // __REFERENCED_H__

