#include "shader.h"
#include "openglhelper.h"
#include "glHeadersInc.h"
#include "openglhelper.h"

#define LOG_FUNTION_ENTRY(fmt, ...) if (m_debugLogiing) { LOG_DEBUG("%s %s(name:%s, " fmt ")", SCHR(this->name()), Q_FUNC_INFO, SCHR(name), __VA_ARGS__); }
#define LOG_EMPTY_FUNTION_ENTRY() if (m_debugLogiing) { LOG_DEBUG("%s %s(name:%s)", SCHR(this->name()), Q_FUNC_INFO, SCHR(name)); }
#define LOCATION(var) \
  int var = glGetUniformLocation(m_programId, SCHR_CONST(name)); \
  if (var == GL_CONSTANTS::InvalidUniformLocation) { LOG_WARNING("%s cannot find location for name %s", SCHR(this->name()), SCHR(name)); return false;}

using namespace  Tools;


using _ec = eBaseErrorCodes;

Shader::Shader(const QString& shaderName, QObject *parent):
  QObject(parent),
  m_lastCode(eBaseErrorCodes::NoError),
  m_name(shaderName),
  m_fragmentShaderId(GL_INVALID_ENUM),
  m_vertexShaderId(GL_INVALID_ENUM),
  m_programId(GL_INVALID_ENUM),
  m_loadDestroyMutex(XMutexModeNonRecursive),
  m_debugLogiing(false)
{
  LOG_DEBUG(QString("new shader %1").arg(name()));
}

Shader::~Shader()
{
  destroy();
}

QString Shader::instanceId() const
{
  return QString().asprintf("[%llX]", uintptr_t(this));
}

QString Shader::name() const
{
  return (m_name.isEmpty()) ? instanceId() : m_name;
}

Tools::eBaseErrorCodes Shader::lastError() const
{
  return m_lastCode;
}

QString Shader::lastErrorReason() const
{
  return m_lastReason;
}

void Shader::enableLogging(bool val)
{
  m_debugLogiing = val;
}

bool Shader::checkFilePath(const QString& path)
{
  return !path.isEmpty() && QFile::exists(path);
}

bool Shader::load(const QString& vertexPath, const QString& fragmentPath)
{
  destroy();
  GLchar logInfo[GL_CONSTANTS::LogBufSize] = {0};
  bool shadersOk = false;

  do
  {
    if (!checkFilePath(vertexPath))
    {
      logError(_ec::Arguments, "vertexPath is empty or invalid");
      break;
    }
    if (!checkFilePath(fragmentPath))
    {
      logError(_ec::Arguments, "fragmentPath is empty or invalid");
      break;
    }
    OpenglHelper glHelper;
    auto timeValue = msTime();
    m_fragmentShaderId = glHelper.makeShader(eShaderType::FragmentShader, fragmentPath, shadersOk);
    LOG_DEBUG("%s make fragment shader %lld", SCHR(name()), (msTime() - timeValue));
    timeValue = msTime();
    if (m_fragmentShaderId == GL_INVALID_ENUM || !shadersOk)
    {
      logError(_ec::CompilationError, "cannot create fragment shader");
      break;
    }

    m_vertexShaderId = glHelper.makeShader(eShaderType::VertexShader, vertexPath, shadersOk);
    LOG_DEBUG("%s, make vertex shader %lld", SCHR(name()), (msTime() - timeValue));
    timeValue = msTime();
    if (m_vertexShaderId == GL_INVALID_ENUM || !shadersOk)
    {
      logError(_ec::CompilationError, "cannot create vertex shader");
      break;
    }

    m_programId = glCreateProgram();
    glAttachShader(m_programId, m_vertexShaderId);
    glAttachShader(m_programId, m_fragmentShaderId);
    glLinkProgram(m_programId);
    GLint success = GL_TRUE;
    glGetProgramiv(m_programId, GL_LINK_STATUS, &success);
    if (success != GL_TRUE)
    {
      glGetProgramInfoLog(m_programId, GL_CONSTANTS::LogBufSize, NULL, logInfo);
      logError(_ec::CompilationError, "cannot create vertex shader");
      break;
    }
    glValidateProgram(m_programId);
    glGetProgramiv(m_programId, GL_VALIDATE_STATUS, &success);
    if (success != GL_TRUE)
    {
      glGetProgramInfoLog(m_programId, GL_CONSTANTS::LogBufSize, NULL, logInfo);
      logError(_ec::CompilationError, "programm validation error");
      break;
    }
    shadersOk = true;
  }
  while(false);
  if (!shadersOk)
  {
    destroy();
  }
  return shadersOk;
}

bool Shader::use()
{
  if (isValid())
  {
    glUseProgram(m_programId);
    return true;
  }

  logError(_ec::ObjectState, "invalid shader");
  return false;
}

bool Shader::isValid()
{
  return (m_programId != GL_CONSTANTS::InvalidId);
}

void Shader::logError(uint e, const QString& reason)
{
  m_lastCode = _ec(e);
  m_lastReason = reason;
  LOG_WARNING(QString("shader %1 error %2. reason: %3").arg(name(), ErrorCodeToString(e), reason));
  emit error(e, reason);
}

void Shader::destroy()
{
  if (m_fragmentShaderId != GL_CONSTANTS::InvalidId)
  {
    glDeleteShader(m_fragmentShaderId);
    m_fragmentShaderId = GL_CONSTANTS::InvalidId;
  }
  if (m_vertexShaderId != GL_CONSTANTS::InvalidId)
  {
    glDeleteShader(m_vertexShaderId);
    m_vertexShaderId = GL_CONSTANTS::InvalidId;
  }
  if (m_programId != GL_CONSTANTS::InvalidId)
  {
    glDeleteProgram(m_programId);
    m_programId = GL_CONSTANTS::InvalidId;
  }
}

bool Shader::setAttribute1f(const QString& name, float v)
{
  LOG_FUNTION_ENTRY("%f", v);
  if (!isValid())
  {
    LOCATION(location)
    glUniform1f(location, v);
    return true;
  }
  logError(_ec::ObjectState, "invalid object state");
  return false;
}

bool Shader::setAttribute1i(const QString& name, int v)
{
  LOG_FUNTION_ENTRY("%d", v);
  if (!isValid())
  {
    LOCATION(location)
    glUniform1i(glGetUniformLocation(m_programId, SCHR_CONST(name)), v);
    return true;
  }
  logError(_ec::ObjectState, "invalid object state");
  return false;
}

bool Shader::setAttribute2f(const QString& name, float v1, float v2)
{
  LOG_FUNTION_ENTRY("%f, %f", v1, v2);
  if (!isValid())
  {
    LOCATION(location)
    glUniform2f(glGetUniformLocation(m_programId, SCHR_CONST(name)), v1, v2);
    return true;
  }
  logError(_ec::ObjectState, "invalid object state");
  return false;
}

bool Shader::setAttribute2i(const QString& name, int v1, int v2)
{
  LOG_FUNTION_ENTRY("%d, %d", v1, v2);
  if (!isValid())
  {
    glUniform2i(glGetUniformLocation(m_programId, SCHR_CONST(name)), v1, v2);
    return true;
  }
  logError(_ec::ObjectState, "invalid object state");
  return false;
}

bool Shader::setAttribute3f(const QString& name, float v1, float v2, float v3)
{
  LOG_FUNTION_ENTRY("%f, %f, %f", v1, v2, v3);
  if (!isValid())
  {
    glUniform3f(glGetUniformLocation(m_programId, SCHR_CONST(name)), v1, v2, v3);
    return true;
  }
  logError(_ec::ObjectState, "invalid object state");
  return false;
}

bool Shader::setAttribute3i(const QString& name, int v1, int v2, int v3)
{
  LOG_FUNTION_ENTRY("%d, %d, %d", v1, v2, v3);
  if (!isValid())
  {
    glUniform3i(glGetUniformLocation(m_programId, SCHR_CONST(name)), v1, v2, v3);
    return true;
  }
  logError(_ec::ObjectState, "invalid object state");
  return false;
}

bool Shader::setAttribute4f(const QString& name, float v1, float v2, float v3, float v4)
{
  LOG_FUNTION_ENTRY("%f, %f, %f, %f", v1, v2, v3, v4);
  if (!isValid())
  {
    glUniform4f(glGetUniformLocation(m_programId, SCHR_CONST(name)), v1, v2, v3, v4);
    return true;
  }
  logError(_ec::ObjectState, "invalid object state");
  return false;
}

bool Shader::setAttribute4i(const QString& name, int v1, int v2, int v3, int v4)
{
  LOG_FUNTION_ENTRY("%d, %d, %d, %d", v1, v2, v3, v4);
  if (!isValid())
  {
    glUniform4i(glGetUniformLocation(m_programId, SCHR_CONST(name)), v1, v2, v3, v4);
    return true;
  }
  logError(_ec::ObjectState, "invalid object state");
  return false;
}


bool Shader::setAttributeMatrix2fv(const QString& name, size_t count, bool transpose, const float *value)
{
  LOG_FUNTION_ENTRY("size: %zu, transpose %s, %llu", count, BOOLYESNO(transpose), uintptr_t(value));
  if (!isValid())
  {
    glUniformMatrix2fv(glGetUniformLocation(m_programId, SCHR_CONST(name)), count, transpose, value);
    return true;
  }
  logError(_ec::ObjectState, "invalid object state");
  return false;
}
bool Shader::setAttributeMatrix3fv(const QString& name, size_t count, bool transpose, const float *value)
{
  LOG_FUNTION_ENTRY("size: %zu, transpose %s, %llu", count, BOOLYESNO(transpose), uintptr_t(value));
  if (!isValid())
  {
    glUniformMatrix3fv(glGetUniformLocation(m_programId, SCHR_CONST(name)), count, transpose, value);
    return true;
  }
  logError(_ec::ObjectState, "invalid object state");
  return false;
}
bool Shader::setAttributeMatrix4fv(const QString& name, size_t count, bool transpose, const float *value)
{
  LOG_FUNTION_ENTRY("size: %zu, transpose %s, %llu", count, BOOLYESNO(transpose), uintptr_t(value));
  if (!isValid())
  {
    glUniformMatrix4fv(glGetUniformLocation(m_programId, SCHR_CONST(name)), count, transpose, value);
    return true;
  }
  logError(_ec::ObjectState, "invalid object state");
  return false;
}

bool Shader::setAttributefv(const QString& name, const float* pv, size_t count, size_t vectorSize)
{
  LOG_FUNTION_ENTRY("%llu, count: %zu, vectorSize: %zu", uintptr_t(pv), count, vectorSize);
  if (pv == nullptr)
  {
    logError(_ec::Arguments, "invalid argument");
    return false;
  }
  if (!isValid())
  {
    logError(_ec::ObjectState, "invalid object state");
    return false;
  }
  switch(vectorSize)
  {
    case 1:{glUniform1fv(glGetUniformLocation(m_programId, SCHR_CONST(name)), count, pv); break;}
    case 2:{glUniform2fv(glGetUniformLocation(m_programId, SCHR_CONST(name)), count, pv); break;}
    case 3:{glUniform3fv(glGetUniformLocation(m_programId, SCHR_CONST(name)), count, pv); break;}
    case 4:{glUniform4fv(glGetUniformLocation(m_programId, SCHR_CONST(name)), count, pv); break;}
    default:{logError(_ec::Arguments, "invalid vector size");return false;}
  }
  return true;
}

bool Shader::setAttributeiv(const QString& name, const int* pv, size_t count, size_t vectorSize)
{
  LOG_FUNTION_ENTRY("%llu, count: %zu, vectorSize: %zu", uintptr_t(pv), count, vectorSize);
  if (pv == nullptr)
  {
    logError(_ec::Arguments, "invalid argument");
    return false;
  }
  if (!isValid())
  {
    logError(_ec::ObjectState, "invalid object state");
    return true;
  }
  switch(vectorSize)
  {
    case 1:{glUniform1iv(glGetUniformLocation(m_programId, SCHR_CONST(name)), count, pv); break;}
    case 2:{glUniform2iv(glGetUniformLocation(m_programId, SCHR_CONST(name)), count, pv); break;}
    case 3:{glUniform3iv(glGetUniformLocation(m_programId, SCHR_CONST(name)), count, pv); break;}
    case 4:{glUniform4iv(glGetUniformLocation(m_programId, SCHR_CONST(name)), count, pv); break;}
    default:{logError(_ec::Arguments, "invalid vector size");return false;}
  }
  return true;
}

bool Shader::setCurrentProjectionMatrix(const QString& name)
{
  LOG_EMPTY_FUNTION_ENTRY();
  bool bRet = false;
  do
  {
    if (name.isEmpty())
    {
      logError(_ec::Arguments, "empty name");
      break;
    }
    if (!isValid())
    {
      logError(_ec::ObjectState, "invalid object state");
      break;
    }
  }
  while(false);
  return bRet;
}

bool Shader::setCurrentModelMatrix(const QString& name)
{
  LOG_EMPTY_FUNTION_ENTRY();
  bool bRet = false;
  do
  {
    if (name.isEmpty())
    {
      logError(_ec::Arguments, "empty name");
      break;
    }
    if (!isValid())
    {
      logError(_ec::ObjectState, "invalid object state");
      break;
    }
  }
  while(false);
  return bRet;
}
