#include "basedirs.h"
#include "toolsmacro.h"
#include "buildConstants.h"

using namespace Tools;

namespace
{
CONST_LITERAL DEFAULT_RC_HOME   ("/");
CONST_LITERAL DEFAULT_IMG_HOME  ("skin/img/");
CONST_LITERAL DEFAULT_LOG_HOME  ("log/");
};

BaseDirs& BaseDirs::instance()
{
    static BaseDirs self;
    return self;
}

BaseDirs::BaseDirs():
    QObject(nullptr)
{
  params.insert("APP_NAME", QString(TARGET_NAME));
  params.insert("IMG_HOME", QString(DEFAULT_IMG_HOME));
  params.insert("RC_HOME", QString(DEFAULT_RC_HOME));
  params.insert("LOG_HOME", QString(DEFAULT_LOG_HOME));
  params.insert("APP_HOME", "./");
  params.insert("APP_PATH", params.value("APP_HOME") + params.value("APP_NAME"));
}

void BaseDirs::setExePath(const QString& path)
{
  instance().params.remove("APP_PATH");
  instance().params.insert("APP_PATH", QString(path));

  QString temp(path);
  int lastSlash = path.lastIndexOf('\\');
  instance().params.remove("APP_HOME");
  instance().params.insert("APP_HOME", temp.left(lastSlash + 1));

  instance().params.remove("IMG_HOME");
  instance().params.insert("IMG_HOME", QString(instance().params.value("APP_HOME")).append(QString(DEFAULT_IMG_HOME)));

  instance().params.remove("RC_HOME");
  instance().params.insert("RC_HOME", QString(instance().params.value("APP_HOME")).append(QString(DEFAULT_RC_HOME)));

  instance().params.remove("LOG_HOME");
  instance().params.insert("LOG_HOME", QString(instance().params.value("APP_HOME")).append(QString(DEFAULT_LOG_HOME)));
}

QString BaseDirs::getSChome()
{
  return instance().params.value("APP_HOME");
}

QString BaseDirs::getRChome()
{
  return instance().params.value("RC_HOME");
}

QString BaseDirs::getIMGhome()
{
  return instance().params.value("IMG_HOME");
}

QString BaseDirs::getLogDir()
{
  return instance().params.value("LOG_HOME");
}


