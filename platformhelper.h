#ifndef PLATFORMHELPER_H
#define PLATFORMHELPER_H
#include <QString>
#include <QHash>

#include "toolsmacro.h"

namespace Tools
{

class PlatformHelper
{
public:
  static quint64 getPid();
  static QString getAppInstanceID();
  static QString getShortInstanceID();
  static int getProcessorsCount();
  static QHash<QString, quint64> getFullMemInfo();
  static quint64 getTotalMem();
  static quint64 getFreeMem();
  static quint64 getAvailableMem();
  static quint64 getMemLoad();
  static QString getOSVersion();
private:
  DISABLE_INSTANTIATING(PlatformHelper)

  static QHash<QString, quint64> getLinuxMeminfo();
  static QHash<QString, quint64> getWindowsMeminfo();
  static QByteArray getInstanceIDData();
};
}
#endif // PLATFORMHELPER_H
