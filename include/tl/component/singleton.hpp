#ifndef _SINGLETON_HPP
#define _SINGLETON_HPP
#include <cassert>
namespace tl::blacklist {
template <typename T, bool heap_alloc = true>
class Singleton {
 protected:
  static T *m_inst_;

 public:
  Singleton() {
    if (!heap_alloc) {
      assert(!m_inst_);
      m_inst_ = static_cast<T *>(this);
    }
  }
  ~Singleton() {
    assert(m_inst_);
    m_inst_ = 0;
  }
  static T *Instance() {
    if (heap_alloc && !m_inst_) {
      m_inst_ = new T();
    }
    return (m_inst_);
  }
  Singleton &operator=(const Singleton &) =  delete;
  Singleton(const Singleton &) = delete;
};
template <typename T, bool heap_alloc>
T *Singleton<T, heap_alloc>::m_inst_ = nullptr;
}  // namespace tl::blacklist

#endif
