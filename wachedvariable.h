#ifndef WACHEDVARIABLE_H
#define WACHEDVARIABLE_H

#include "qelapsedtimer.h"
#include "qglobal.h"
#include "qthread.h"
#include "qvariant.h"
#include "tools.h"
#include "toolsmacro.h"
#include "xmutex.h"
#include "xmutexguard.h"
#include <chrono>
#include <thread>
namespace Tools
{
class WachedVariableNotifier: public QObject
{
    Q_OBJECT
public:
    explicit WachedVariableNotifier(QObject *parent = nullptr):QObject(parent){}
signals:
    void valueChanged();
};

template <typename T>
class WachedVariable
{
public:
    explicit WachedVariable(QObject *parent = nullptr):m_notifier(parent) {}
    WachedVariable(const T& otherval):m_value(otherval){}
    WachedVariable(const WachedVariable& other):m_value(other.m_value){}
    WachedVariable(const WachedVariable&& other) = delete;
    WachedVariable& operator=(const T& v)const{set(v);return *this;}
    WachedVariable& operator=(const WachedVariable& other)
    {
        if(this != &other)
        {
            set(other.m_value);
        }
        return *this;
    }

    bool operator==(const WachedVariable& other)const
    {
        XMutexGuard g(&m_mutex);
        return m_value == other.m_value;
    }
    bool operator!=(const WachedVariable& other)const
    {
        XMutexGuard g(&m_mutex);
        return m_value != other.m_value;
    }

    T get() const
    {
        XMutexGuard g(&m_mutex);
        return m_value;
    }

    // Warning! thread unsafe
    T& rget() {return m_value;}
    const T& crget() const {return m_value;}

    WachedVariableNotifier* notifier() {return &m_notifier;}
    void set(const T& v)
    {
        XMutexGuard g(&m_mutex);
        if(m_value != v)
        {
            m_value = v;
            emit m_notifier.valueChanged();
        }
    }
    bool waitForValue(const T& v, qint64 mstimeout = 0) const
    {
        QElapsedTimer timer;
        XMUTEX_LOCK(m_mutex);
        T last = m_value;
        XMUTEX_UNLOCK(m_mutex);
        while((v != last) && (mstimeout > 0) && (timer.elapsed() <= mstimeout))
        {
            mssleep(10);

            XMUTEX_LOCK(m_mutex);
            last = m_value;
            XMUTEX_UNLOCK(m_mutex);
        }
        return (mstimeout <= 0) || (timer.elapsed() >= mstimeout);
    }

private:
    T m_value;
    WachedVariableNotifier m_notifier;
    mutable XNonRecursiveMutex m_mutex;
};

typedef WachedVariable<int> WachedInt;
typedef WachedVariable<float> WachedFloat;
typedef WachedVariable<double> WachedDouble;
typedef WachedVariable<bool> WachedBool;

// Qt types
typedef WachedVariable<qint64> WachedQint64;
typedef WachedVariable<quint64> WachedQUint64;
typedef WachedVariable<qint32> WachedQint32;
typedef WachedVariable<quint32> WachedQUint32;
typedef WachedVariable<qint16> WachedQint16;
typedef WachedVariable<quint16> WachedQUint16;
typedef WachedVariable<qint8> WachedQint8;
typedef WachedVariable<quint8> WachedQUint8;

typedef WachedVariable<QString> WachedQString;
typedef WachedVariable<QVariant> WachedQVariant;

}; // namespace Tools
#endif // WACHEDVARIABLE_H
