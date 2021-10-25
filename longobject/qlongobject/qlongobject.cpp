#include "qlongobject.h"
#include <cstring>
constexpr quint64 InitialReservedSize(4);

QLongObject::QLongObject(int i):
  m_digit(nullptr),
  m_size(1),
  m_reserved(InitialReservedSize),
  m_neg(i < 0)
{
  m_digit = d_alloc(m_reserved);
  m_digit[0] = uint(qAbs<int>(i));
}

QLongObject::~QLongObject()
{
  d_free();
}

void QLongObject::d_realloc(quint64 newSize)
{
  DigitType* last = m_digit;
  m_digit = d_alloc(newSize);
  m_reserved = newSize;
  if(m_digit==nullptr)
  {
    throw QLongObjectMemoryException("cannot alloc new buffer");
  }
  if(newSize < m_size)
  {
    d_free();
    throw QLongObjectDataLossException("data can be truncated. new buffer size is less than actual data size");
  }
  m_reserved = newSize;
  if(last != nullptr)
  {
    memcpy(m_digit, last, bytesize(m_size));
    delete [] last;
  }
}

QLongObject::DigitType* QLongObject::d_alloc(quint64 newSize)
{
  DigitType* ptr = new DigitType[newSize];
  memset(ptr, 0, newSize * sizeof(DigitType));
  return ptr;
}

void QLongObject::d_free()
{
  if(m_digit != nullptr)
  {
    delete [] m_digit;
    m_digit = nullptr;
  }
  m_reserved = 0;
  m_size = 0;
}
