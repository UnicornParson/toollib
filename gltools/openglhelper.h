#ifndef OPENGLHELPER_H
#define OPENGLHELPER_H
#include <QAtomicInt>
#include "tools.h"
#include "xmutex.h"
#include "openglhelpertypes.h"
#include "version.h"

namespace Tools
{
class OpenglHelperPrivate;
class OpenglHelper
{
public:

  OpenglHelper();
  ~OpenglHelper();

  QString glError();
  void printGLError();

  Version glVersion();
  QString glVersionString();
  void updateVersionFlags();
  QString glVendorString();
  QString glRendererString();
  bool checkVersionFlags(const QList<eVersionFlag>& flags);
  uint makeShader(eShaderType type, const QString& path, bool& ok);
private:
  Q_DISABLE_COPY_MOVE(OpenglHelper)
  static OpenglHelperPrivate* m_private;
  static QAtomicInt m_privateRefs;
  static XMutex m_privateMutex;

};
}
#endif // OPENGLHELPER_H
