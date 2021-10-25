#ifndef VECTORWRAPPER_H
#define VECTORWRAPPER_H
#include "vector.h"
//=========================================================================================================================================
// Vector4Wrapper generator
//=========================================================================================================================================
template< typename type >
class Vector4Wrapper
{
public:
  Vector4Wrapper(const QString& name, IVectorChangeObserver* observer = nullptr):
    m_observer(observer),
    m_name(name) {}

  explicit Vector4Wrapper(const Vector4Wrapper<type>& other):
    m_observer(other.m_observer),
    m_vector(other.m_vector),
    m_name(other.m_name) {}

  inline Vector4<type>& getRef() {return m_vector;}
  inline void SetX(type v)
  {
    if (m_vector.x() != v)
    {
      m_vector.x() = v;
      if (m_observer != nullptr)
      {
        m_observer->onVecotorChanged(m_name, VectorParamName::X);
      }
    }
  }

  inline void SetY(type v)
  {
    if (m_vector.y() != v)
    {
      m_vector.y() = v;
      if (m_observer != nullptr)
      {
        m_observer->onVecotorChanged(m_name, VectorParamName::Y);
      }
    }
  }

  inline void SetZ(type v)
  {
    if (m_vector.z() != v)
    {
      m_vector.z() = v;
      if (m_observer != nullptr)
      {
        m_observer->onVecotorChanged(m_name, VectorParamName::Z);
      }
    }
  }

  inline void SetW(type v)
  {
    if (m_vector.w() != v)
    {
      m_vector.w() = v;
      if (m_observer != nullptr)
      {
        m_observer->onVecotorChanged(m_name, VectorParamName::W);
      }
    }
  }

  inline type x() const {return m_vector.d[0];}
  inline type y() const {return m_vector.d[1];}
  inline type z() const {return m_vector.d[2];}
  inline type w() const {return m_vector.d[3];}
  inline QString name() const {return m_name;}
  inline void set(type _x, type _y, type _z, type _w)
  {
    m_vector.x() = _x;
    m_vector.y() = _y;
    m_vector.z() = _z;
    m_vector.w() = _w;
    if (m_observer != nullptr)
    {
      m_observer->onVecotorChanged(m_name, VectorParamName::ANY);
    }
  }

  inline void set(type val)
  {
    set(val, val, val, val);
  }

  inline void set(Vector4<type> vector)
  {
    m_vector = vector;
    if (m_observer != nullptr)
    {
      m_observer->onVecotorChanged(m_name, VectorParamName::ANY);
    }
  }
  inline void add(type v)
  {
    m_vector += v;
    if (m_observer != nullptr)
    {
      m_observer->onVecotorChanged(m_name, VectorParamName::ANY);
    }
  }
  inline void reduce(type v)
  {
    m_vector -= v;
    if (m_observer != nullptr)
    {
      m_observer->onVecotorChanged(m_name, VectorParamName::ANY);
    }
  }
protected:
  IVectorChangeObserver* m_observer; \
  Vector4<type> m_vector;
  QString m_name;
};

//=========================================================================================================================================
// Vector3Wrapper generator
//=========================================================================================================================================
template< typename type >
class Vector3Wrapper
{
public:
  Vector3Wrapper(const QString& name, IVectorChangeObserver* observer = nullptr):
    m_observer(observer),
    m_name(name) {}

  explicit Vector3Wrapper(const Vector3Wrapper<type>& other):
    m_observer(other.m_observer),
    m_vector(other.m_vector),
    m_name(other.m_name) {}

  inline Vector3<type>& getRef() {return m_vector;}
  inline void SetX(type v)
  {
    if (m_vector.x() != v)
    {
      m_vector.x() = v;
      if (m_observer != nullptr)
      {
        m_observer->onVecotorChanged(m_name, VectorParamName::X);
      }
    }
  }

  inline void SetY(type v)
  {
    if (m_vector.y() != v)
    {
      m_vector.y() = v;
      if (m_observer != nullptr)
      {
        m_observer->onVecotorChanged(m_name, VectorParamName::Y);
      }
    }
  }

  inline void SetZ(type v)
  {
    if (m_vector.z() != v)
    {
      m_vector.z() = v;
      if (m_observer != nullptr)
      {
        m_observer->onVecotorChanged(m_name, VectorParamName::Z);
      }
    }
  }

  inline type x() const {return m_vector.d[0];}
  inline type y() const {return m_vector.d[1];}
  inline type z() const {return m_vector.d[2];}
  inline QString name() const {return m_name;}
  inline void set(type _x, type _y, type _z)
  {
    m_vector.x() = _x;
    m_vector.y() = _y;
    m_vector.z() = _z;
    if (m_observer != nullptr)
    {
      m_observer->onVecotorChanged(m_name, VectorParamName::ANY);
    }
  }

  inline void set(type val)
  {
    set(val, val, val);
  }

  inline void set(Vector3<type> vector)
  {
    m_vector = vector;
    if (m_observer != nullptr)
    {
      m_observer->onVecotorChanged(m_name, VectorParamName::ANY);
    }
  }
  inline void add(type v)
  {
    m_vector += v;
    if (m_observer != nullptr)
    {
      m_observer->onVecotorChanged(m_name, VectorParamName::ANY);
    }
  }
  inline void reduce(type v)
  {
    m_vector -= v;
    if (m_observer != nullptr)
    {
      m_observer->onVecotorChanged(m_name, VectorParamName::ANY);
    }
  }
protected:
  IVectorChangeObserver* m_observer; \
  Vector3<type> m_vector;
  QString m_name;
};



//=========================================================================================================================================
// create wrappers
//=========================================================================================================================================
//Vector4Wrapper(double, d)
//Vector4Wrapper(float, f)
//Vector4Wrapper(uint, ui)
//Vector4Wrapper(int, i)

typedef Vector4Wrapper<double> Vector4dWrapper;
typedef Vector4Wrapper<float> Vector4fWrapper;
typedef Vector4Wrapper<uint> Vector4uiWrapper;
typedef Vector4Wrapper<int> Vector4iWrapper;
typedef Vector4Wrapper<quint64> Vector4ui64Wrapper;
typedef Vector4Wrapper<qint64> Vector4i64Wrapper;

typedef Vector3Wrapper<double> Vector3dWrapper;
typedef Vector3Wrapper<float> Vector3fWrapper;
typedef Vector3Wrapper<uint> Vector3uiWrapper;
typedef Vector3Wrapper<int> Vector3iWrapper;
typedef Vector3Wrapper<quint64> Vector3ui64Wrapper;
typedef Vector3Wrapper<qint64> Vector3i64Wrapper;
#endif // VECTORWRAPPER_H
