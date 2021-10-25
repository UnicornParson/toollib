#ifndef RESPONSEOBJECT_H
#define RESPONSEOBJECT_H
#include "tools.h"
#include "memcontrol.h"
#include <QObject>

namespace Tools
{
// TODO: remove me
GARBAGE_CODE_MARKER
#if 0
class ResponseErrorReason: public QObject
{
    Q_OBJECT
    Q_ENUMS(e)
public:
    enum e
    {
        NoError = 0, // all is ok
        Arguments, // invalid arguments are provided to function
        ObjectState, // object in invalid state (not initialized?)
        Configuration, // incorrect configuraton that required for operation
        Network, // network error
        RemoteError, //network is ok but back-end returns error code
        Unknown, // anything else
        EXTENDED_REASON_CODE = 128 // end of universal reasons list
    };
    Q_ENUM(e)

    static QString toString(ResponseErrorReason::e val)
    {
        return QEnumToString<ResponseErrorReason::e>(val);
    }

    static bool isUniversalReason(int val)
    {
        return (val >= e::NoError) && (val < e::EXTENDED_REASON_CODE);
    }

private:
    explicit ResponseErrorReason(QObject *parent = nullptr): QObject(parent) {}
};
typedef ResponseErrorReason::e eResponseErrorReason;
#endif
// object reference container like "optional"
// is not owner of object
// cannot be userd for return local object from function

template< class T >
class ResponseObject
{
public:
    explicit ResponseObject(const T& obj):
        m_objRef(obj),
        m_isValid(true),
        m_invalidReason(0)
    { ADD_POINTER_SENSOR(this); }

    ResponseObject(const ResponseObject<T>& other):
        m_objRef(other.m_objRef),
        m_isValid(other.m_isValid),
        m_invalidReason(other.m_invalidReason),
        m_errorString(other.m_errorString)
    { ADD_POINTER_SENSOR(this); }

    ~ResponseObject() { REMOVE_POINTER_SENSOR(this); }
    inline operator T&() { return m_objRef; }
    inline operator bool() const { return isValid(); }
    inline bool operator==(const ResponseObject& other) const
    {
        return (m_objRef == other.m_objRef) &&
                (m_isValid == other.m_isValid) &&
                (m_invalidReason == other.m_invalidReason) &&
                (m_errorString == other.m_errorString);
    }

    inline ResponseObject& operator=(const ResponseObject& other)
    {
        if (this != &other)
        {
            m_objRef = other.m_objRef;
            m_isValid = other.m_isValid;
            m_invalidReason = other.m_invalidReason;
            m_errorString = other.m_errorString;
        }
        return *this;
    }

    inline bool isValid() const { return  m_isValid; }
    inline void setValid(bool val) { m_isValid = val; }

    inline int reason() const { return m_invalidReason; }
    inline void setReason(int val) { m_invalidReason = val; }

    inline QString errorString() const { return m_errorString; }
    inline void setErrorString(const QString& str) {m_errorString = str;}
    inline void appendErrorString(const QString& str) {m_errorString += str;}
    inline void resetErrorString() {m_errorString.clear();}

    void reset()
    {
        m_isValid = false;
        m_invalidReason = 0;
        resetErrorString();
    }

private:
    T& m_objRef;
    bool m_isValid;
    int m_invalidReason;
    QString m_errorString;
}; // class ResponseObject


// object container like "optional"
// IT IS owner of object
// can be returned from function
// object must support creation and copy contruector/operator

template< class T >
class ResponseObjectContainer
{
public:
    explicit ResponseObjectContainer(const T& obj = T()):
        m_obj(obj),
        m_isValid(true),
        m_invalidReason(0)
    { ADD_POINTER_SENSOR(this); }

    ResponseObjectContainer(const ResponseObject<T>& other):
        m_obj(other.m_obj),
        m_isValid(other.m_isValid),
        m_invalidReason(other.m_invalidReason),
        m_errorString(other.m_errorString)
    { ADD_POINTER_SENSOR(this); }

    ~ResponseObjectContainer() { REMOVE_POINTER_SENSOR(this); }
    inline operator T&() { return m_obj; }
    inline operator bool() const { return isValid(); }
    inline bool operator==(const ResponseObjectContainer<T>& other) const
    {
        return (m_obj == other.m_obj) &&
                (m_isValid == other.m_isValid) &&
                (m_invalidReason == other.m_invalidReason) &&
                (m_errorString == other.m_errorString);
    }

    inline ResponseObjectContainer<T>& operator=(const ResponseObjectContainer<T>& other)
    {
        if (this != &other)
        {
            m_obj = other.m_obj;
            m_isValid = other.m_isValid;
            m_invalidReason = other.m_invalidReason;
            m_errorString = other.m_errorString;
        }
        return *this;
    }

    inline bool isValid() const { return  m_isValid; }
    inline void setValid(bool val) { m_isValid = val; }

    inline int reason() const { return m_invalidReason; }
    inline void setReason(int val) { m_invalidReason = val; }

    inline QString errorString() const { return m_errorString; }
    inline void setErrorString(const QString& str) {m_errorString = str;}
    inline void appendErrorString(const QString& str) {m_errorString += str;}
    inline void resetErrorString() {m_errorString.clear();}

    void reset()
    {
        m_isValid = false;
        m_invalidReason = 0;
        resetErrorString();
    }

private:
    T m_obj;
    bool m_isValid;
    int m_invalidReason;
    QString m_errorString;
}; // class ResponseObjectContainer
} // namespace Tools
#endif // RESPONSEOBJECT_H
