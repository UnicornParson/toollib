#ifndef FUNCTIONSCHEDULER_H
#define FUNCTIONSCHEDULER_H

#include <QAtomicInt>
#include <functional>
#include <stdexcept>

namespace Tools
{
  class FunctionScheduler
  {
  public:
    using Routine_t = std::function<void()>;
    using ExceptonCallback_t = std::function<void(const std::exception&)>;
    using RuntimeErrorCallback_t = std::function<void(const std::runtime_error&)>;
    static inline bool isEmpty() {return (refs() == 0);}
    static inline int refs() { return self().m_refs;}
    static void destroy();
    static void scheduleOnce(const Routine_t& function,
                             int ms,
                             const ExceptonCallback_t& ec = [](const std::exception&){},
                             const RuntimeErrorCallback_t& rec = [](const std::runtime_error&){});

    static void setInterval(const Routine_t& function,
                            int ms,
                            const ExceptonCallback_t& ec = [](const std::exception&){},
                            const RuntimeErrorCallback_t& rec = [](const std::runtime_error&){});

    static inline bool verbose() {return self().m_verbose;}
    static inline void setVerbose(bool val) {self().m_verbose = val;}
    static void reset();
  private:

    static FunctionScheduler& self();
    static void safeExec(const Routine_t& function,
                         const ExceptonCallback_t& ec,
                         const RuntimeErrorCallback_t& rec);

    static void delayImpl(const Routine_t& function,
                          int ms,
                          const ExceptonCallback_t& ec,
                          const RuntimeErrorCallback_t& rec);

    FunctionScheduler();
    FunctionScheduler(const FunctionScheduler&) = default;
    FunctionScheduler(const FunctionScheduler&&) = delete;
    ~FunctionScheduler();

    bool m_destroyFlag;
    bool m_verbose;
    QAtomicInt m_refs;
  }; // class FunctionScheduler

  inline void scheduleOnce(const FunctionScheduler::Routine_t& function,
                           int ms,
                           const FunctionScheduler::ExceptonCallback_t& ec = [](const std::exception&){},
                           const FunctionScheduler::RuntimeErrorCallback_t& rec = [](const std::runtime_error&){})
  {
    FunctionScheduler::scheduleOnce(function, ms, ec, rec);
  }

  inline void setInterval(const FunctionScheduler::Routine_t& function,
                          int ms,
                          const FunctionScheduler::ExceptonCallback_t& ec = [](const std::exception&){},
                          const FunctionScheduler::RuntimeErrorCallback_t& rec = [](const std::runtime_error&){})
  {
    FunctionScheduler::setInterval(function, ms, ec, rec);
  }

} // namespace Tools
#endif // FUNCTIONSCHEDULER_H
