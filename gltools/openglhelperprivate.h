#ifndef OPENGLHELPERPRIVATE_H
#define OPENGLHELPERPRIVATE_H
#include "version.h"
#include "tools.h"
#include <QBitArray>
#include <QObject>
#include "openglhelpertypes.h"
#include "glHeadersInc.h"


namespace Tools
{
  class OpenglHelperPrivate
  {
  public:
    OpenglHelperPrivate();
    void updateVersionFlags();
    bool checkVersionFlags(const QList<eVersionFlag>& flags);
    Version glVersion();
    QString glVersionString();
    QString glVendorString();
    QString glRendererString();

    INLINE QString glError()
    {
      return QString().asprintf("%s", glewGetErrorString(glGetError()));
    }

    INLINE void printGLError()
    {
      GLenum e = glGetError();
      if (e == GL_NO_ERROR)
      {
        qDebug() << glError();
      }
      else
      {
        qWarning() << glError();
      }
    }
    uint makeShader(eShaderType type, const QString& path, bool& ok);
  private:
    QBitArray m_versionFlags;
  }; // class OpenglHelperPrivate
} // namespace Tools
#endif // OPENGLHELPERPRIVATE_H
