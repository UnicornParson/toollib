#ifndef SHADER_H
#define SHADER_H
#include <QString>
#include <QObject>
#include "toolsmacro.h"
#include "baseerrorcodes.h"
#include "xmutex.h"

namespace DefaultAttributes
{
CONST_LITERAL ProjectionMatrixAttributeDefaultName("projectionMatrix");
CONST_LITERAL ModelMatrixAttributeDefaultName("modelMatrix");
};

class Shader: public QObject
{
  Q_OBJECT


public:
  static INLINE QString ErrorCodeToString(uint e)
  {
    return Tools::BaseErrorCodes::toString(Tools::eBaseErrorCodes(e));
  }

  Shader(const QString& shaderName = QString(), QObject *parent = nullptr);
  virtual ~Shader();
  virtual bool load(const QString& vertexPath, const QString& fragmentPath);
  virtual bool use();
  virtual bool isValid();
  QString instanceId() const;
  QString name() const;
  Tools::eBaseErrorCodes lastError() const;
  QString lastErrorReason() const;
  void enableLogging(bool val);

  virtual bool setCurrentProjectionMatrix(const QString& name = DefaultAttributes::ProjectionMatrixAttributeDefaultName);
  virtual bool setCurrentModelMatrix(const QString& name = DefaultAttributes::ProjectionMatrixAttributeDefaultName);

  virtual bool setAttribute1f(const QString& name, float v);
  virtual bool setAttribute1i(const QString& name, int v);
  virtual bool setAttribute2f(const QString& name, float v1, float v2);
  virtual bool setAttribute2i(const QString& name, int v1, int v2);
  virtual bool setAttribute3f(const QString& name, float v1, float v2, float v3);
  virtual bool setAttribute3i(const QString& name, int v1, int v2, int v3);
  virtual bool setAttribute4f(const QString& name, float v1, float v2, float v3, float v4);
  virtual bool setAttribute4i(const QString& name, int v1, int v2, int v3, int v4);

  virtual bool setAttributeMatrix2fv(const QString& name, size_t count, bool transpose, const float *value);
  virtual bool setAttributeMatrix3fv(const QString& name, size_t count, bool transpose, const float *value);
  virtual bool setAttributeMatrix4fv(const QString& name, size_t count, bool transpose, const float *value);

  virtual bool setAttributefv(const QString& name, const float* pv, size_t count, size_t vectorSize);
  virtual bool setAttributeiv(const QString& name, const int *pv, size_t count, size_t vectorSize);

signals:
  void error(uint e, const QString& reason);

protected:
  virtual void destroy();
  void logError(uint e, const QString& reason);
  static bool checkFilePath(const QString& path);

  Tools::eBaseErrorCodes m_lastCode;
  QString m_lastReason;
  QString m_name;
  uint m_fragmentShaderId;
  uint m_vertexShaderId;
  uint m_programId;
  Tools::XMutex m_loadDestroyMutex;
  bool m_debugLogiing;
};


#endif // SHADER_H
