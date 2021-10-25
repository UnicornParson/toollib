#ifndef QLONGOBJECT_H
#define QLONGOBJECT_H

#include <QtGlobal>
#include <QString>
#include <QDebug>
class QLongObjectMemoryException : public std::exception
{
public:
  explicit QLongObjectMemoryException(const QString& s):reason(s)
  {
    qWarning() << "new QLongObjectMemoryException " << s;
  }
  QString reason;
};

class QLongObjectDataLossException : public std::exception
{
public:
  explicit QLongObjectDataLossException(const QString& s):reason(s)
  {
    qWarning() << "new QLongObjectDataLossException " << s;
  }
  QString reason;
};

class QLongObject
{
public:
  using DigitType = uint;
  QLongObject(int i = 0);
  ~QLongObject();
protected:
  void d_realloc(quint64 newSize);
  void d_free();
  inline static bool overflow(DigitType d)
  {
    constexpr DigitType OverflowMask(1 << ((sizeof (DigitType) * 8) - 1));
    return (0 != (d & OverflowMask));
  }
  inline static quint64 bytesize(quint64 elemCount)
  {
    return elemCount*sizeof(DigitType);
  }
  DigitType* d_alloc(quint64 newSize);
  DigitType* m_digit;
  quint64 m_size;
  quint64 m_reserved;
  bool m_neg;
};

#endif // QLONGOBJECT_H
