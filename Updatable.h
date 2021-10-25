#ifndef UPDATABLE_H
#define UPDATABLE_H
#include <functional>
#include <thread>
#include "sharedxmutex.h"
namespace Tools
{
  template< class T >
  class Updatable
  {
  public:
    struct UpdatableCtx
    {
      T prev;
      Updatable<T> *owner;
    };

    typedef std::function<T(const UpdatableCtx&)> UpdaterFunct_t;
    typedef std::function<void(const T& /*new*/, const T&/*prev*/)> OnUpdateFinishedFunct_t;
    typedef OnUpdateFinishedFunct_t OnValueUpdatedFunct_t;

    explicit Updatable(const T &val):
      m_value(val),
      m_interrupt(false),
      m_activeThread(false),
      m_thread(nullptr),
      mf_updater(Updatable<T>::defaultUpdater),
      mf_onUpdated(Updatable<T>::defaultCallback),
      mf_onUpdateFinished(Updatable<T>::defaultCallback)
    {}

    ~Updatable()
    {
      if (m_thread != nullptr)
      {
        delete m_thread;
      }
    }

    inline void set(const T &val)
    {
      SXMUTEX_LOCK_WRITE(m_mutex);
      T prev = m_value;
      m_value = val;
      SXMUTEX_UNLOCK_WRITE(m_mutex);
      if (prev != val)
      {
        mf_onUpdated(val, prev);
      }
    }

    inline T get()
    {
      SXMUTEX_LOCK_READ(m_mutex);
      T ret = m_value;
      SXMUTEX_UNLOCK_READ(m_mutex);
      return ret;
    }

    inline operator T()
    {
      return get();
    }

    /// \brief updater thread body should return updated value
    void setUpdater(const UpdaterFunct_t& updater) { mf_updater = updater;}

    /// \brief value changed callback
    /// \details will be called when value is changed
    void setOnValueUpdatedFunct(const OnValueUpdatedFunct_t& onUpdatedFunc) { mf_onUpdated = onUpdatedFunc;}

    /// \brief updater function exit callback
    /// \details will be called when updater thread is finished. previous value will never be used. it can be deleted
    void setOnUpdateFinishedFunct(const OnUpdateFinishedFunct_t& onUpdateFinishedFunc) { mf_onUpdateFinished = onUpdateFinishedFunc;}

    /// \brief start async update
    void requestUpdate(const UpdaterFunct_t& updater)
    {
      setUpdater(updater);
      requestUpdate();
    }

    /// \brief request updater thread interruption
    inline void interruptUpdate() {m_interrupt.store(true);}

    /// \brief returns true if interruption is requested
    inline bool isInterrupted() const {return m_interrupt.load();}

    /// \brief start async update with already setted updater function
    void requestUpdate()
    {
      if (m_thread != nullptr)
      {
        m_thread->join();
        delete m_thread;
        m_thread = nullptr;
      }
      m_thread = new std::thread(&Updatable::updaterWrp, this);
    }

    /// \brief lock value.
    /// \details lock the current value. it cannot be rewrited by updater or set function
    inline void freeze(){SXMUTEX_LOCK_WRITE(m_mutex);}

    /// \brief unlock value.
    inline void defreeze(){SXMUTEX_LOCK_WRITE(m_mutex);}

    inline bool isActive()
    {
      LOG_TRACE("m_activeThread: %s, m_thread: %s", boolToString(m_activeThread.load()),ISNULL2STR(m_thread) );
      return (m_activeThread.load() && m_thread != nullptr);
    }

    void wait()
    {
      if (!isActive())
      {
        LOG_TRACE("updater is not active or null");
        return;
      }
      m_thread->join();
    }
  protected:
    Q_DISABLE_COPY_MOVE(Updatable)

    void updaterWrp()
    {
      try
      {
        m_activeThread.store(true);
        UpdatableCtx ctx;
        ctx.prev = get();
        ctx.owner = this;
        T newVal = mf_updater(ctx);
        set(newVal);
        mf_onUpdateFinished(newVal, ctx.prev);
        m_activeThread.store(false);
      }
      catch (...)
      {
        LOG_ERROR("something is wrong");
      }
    }

    T m_value;
    SharedXMutex m_mutex;
    std::atomic_bool m_interrupt;
    std::atomic_bool m_activeThread;
    std::thread *m_thread;

    //callbacks
    static T defaultUpdater(const UpdatableCtx& ctx){return ctx.prev;}
    static void defaultCallback(const T& /*new*/, const T&/*prev*/){}
    UpdaterFunct_t mf_updater;
    OnValueUpdatedFunct_t mf_onUpdated;
    OnUpdateFinishedFunct_t mf_onUpdateFinished;



  };
}
#endif // UPDATABLE_H
