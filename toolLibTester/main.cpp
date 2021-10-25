#include <QCoreApplication>
#include "securityutils.h"
#include "test/toolstest.h"
#include "tools.h"
#include <iostream>
#include <QTextCodec>
#include <QFile>
#include <QDir>

#define printfErr(...) fprintf(stderr, __VA_ARGS__)

using namespace Tools;

namespace
{
  CONST_LITERAL LOGS_BASE_DIR("logs/");
  CONST_LITERAL LOGS_CFG_FILENAME("log.cfg");
  CONST_LITERAL LOG_FILENAME("console.log");
}

int main(int argc, char *argv[])
{
  int rc = 255;
  QCoreApplication a(argc, argv);
  do
  {
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QString path = QString(argv[0]);
    int lastSlash = path.lastIndexOf('\\');
    QString binDir = path.left(lastSlash + 1);
    QString logDir = CTools::pathNormalize(binDir + LOGS_BASE_DIR + QDateTime::currentDateTime().toString("dd.MM.yyyy_hh.mm.ss"));
    if(!QDir().exists(logDir))
    {
      bool mkdirRc = QDir().mkdir(logDir);
      if(!mkdirRc)
      {
        printfErr("Cannot create log directory in %s", SCHR(logDir));
        break;
      }
    }
    if(!SecurityUtils::init())
    {
      LOG_ERROR("cannot init SecurityUtils");
break;
    }
    else
    {
      LOG_INFO("SecurityUtils test OK");
    }
    ToolsTest testEngine;
    bool testRc = testEngine.run();
    testEngine.printResults();
    if(testRc)
    {
      LOG_INFO("Self test PASSED");
      rc = 0;
    }
    else
    {
      LOG_ERROR("Self test FAILED");
      rc = 255;
    }
  }
  while(false);
  return rc;
}
