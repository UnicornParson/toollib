#ifndef LOGUTILS_H
#define LOGUTILS_H
#include "CuteLogger/cuteloggerinc.h"
#include <QRect>
#include <QString>

inline QString QRect2Str(const QRect& r)
{
  return QString("QRect(x:%1;y:%2;w:%3;h:%4)").arg(r.x()).arg(r.y()).arg(r.width()).arg(r.height());
}

inline QString QRectF2Str(const QRectF& r)
{
  return QString("QRectF(x:%1;y:%2;w:%3;h:%4)").arg(r.x()).arg(r.y()).arg(r.width()).arg(r.height());
}

inline void PrintQRect(const QRect& r)
{
  LOG_TRACE(QRect2Str(r));
}

inline void PrintQRectF(const QRect& r)
{
  LOG_TRACE(QRectF2Str(r));
}

#endif // LOGUTILS_H
