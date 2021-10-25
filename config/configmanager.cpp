#include "configmanager.h"
#include "CuteLogger/cuteloggerinc.h"
#include "helpers.h"
using namespace Tools;

#define CATEGORY_XML_TAG QString("category")
ConfigManager ConfigManager::defaultManager;

ConfigManager::ConfigManager()
{

}

ConfigManager::~ConfigManager()
{

}

ConfigManager& ConfigManager::getDefaultManager()
{
  return defaultManager;
}

bool ConfigManager::load(const QString& path)
{
  params.clear();
  return append(path);
}

bool ConfigManager::append(const QString& path)
{
  bool ret = false;
  do
  {
    if(path.isEmpty())
    {
      LOG_WARNING("path is empty");
      break;
    }
    LOG_TRACE(QString("Loading configs from %1").arg(path));

    QString npath = Helpers::pathNormalize(path);
    QFile configFile(npath);
    bool rc = configFile.open(QIODevice::ReadOnly);
    if(!rc)
    {
      LOG_WARNING(QString("Cannot open file %1").arg(npath));
      break;
    }

    QDomDocument domDoc;
    QString errMsg;
    int errorLine = 0;
    int errorColumn = 0;
    LOG_TRACE(QString("Try to load %1").arg(npath));

    QString XMLData =  QString(configFile.readAll());
    configFile.close();
    if(false == domDoc.setContent(XMLData, &errMsg, &errorLine, &errorColumn))
    {
      LOG_WARNING(QString("XML parsing error. [%1] line %2, column %3")
                  .arg(errMsg)
                  .arg(errorLine)
                  .arg(errorColumn));
      LOG_TRACE_LINE;
      LOG_TRACE(XMLData);
      LOG_TRACE_LINE;

      break;
    }

    QDomElement content = domDoc.firstChildElement("config");
    if(content.isNull())
    {
      LOG_WARNING("invalid document. config section doesn't exist");
      break;
    }

    QDomElement element = content.firstChildElement();
    ret = true;
    while(!element.isNull())
    {
      bool rc = false;
      if(element.nodeName().toLower() == CATEGORY_XML_TAG)
      {
        LOG_TRACE(QString("category %1 is found").arg(element.attribute("name")));
        rc = loadCategory(&element);
      }
      else
      {
        LOG_TRACE("param item is found");
        rc = loadParam(&element);
      }
      if(!rc)
      {
        LOG_WARNING("loading error");
        ret = false;
        break;
      }
      element = element.nextSiblingElement();
    }
  }
  while(false);
  return ret;
}

bool ConfigManager::loadParam(QDomElement* element, const QString &currentPath)
{
  bool ret = false;
  do
  {

    if(NULL == element || element->isNull())
    {
      LOG_WARNING("element is null");
      break;
    }

    QString paramName = QString("%1/%2").arg(currentPath, element->nodeName());
    LOG_TRACE(QString("add param %1").arg(paramName));
    params.insert(paramName, QVariant(element->text()));
    ret = true;
    /*
    QDomElement subelement = element->firstChildElement();


    while(!subelement.isNull())
    {
      bool rc = false;
      QString paramName = QString("%1/%2").arg(currentPath, subelement.nodeName());
      LOG_TRACE(QString("add param %1").arg(paramName));
      params.insert(paramName, QVariant(subelement.text()));
      subelement = subelement.nextSiblingElement();
    }
*/
  }
  while(false);
  return ret;
}

bool ConfigManager::loadCategory(QDomElement* element, const QString &currentPath)
{
  bool ret = false;
  do
  {
    if(element->isNull())
    {
      LOG_WARNING("element is null");
      break;
    }
    QString tmpPath(currentPath);
    QString elementName = element->attribute("name");
    if(currentPath.isEmpty())
    {
      tmpPath = elementName;
    }
    else
    {
      tmpPath = QString("%1/%2").arg(currentPath, elementName);
    }
    if(elementName.isEmpty())
    {
      LOG_WARNING("category without name attribute");
      break;
    }
    QDomElement subelement = element->firstChildElement();

    ret = true;
    while(!subelement.isNull())
    {
      bool rc = false;

      if(subelement.nodeName().toLower() == CATEGORY_XML_TAG)
      {
        LOG_TRACE(QString("category %1 is found in %2").arg(subelement.attribute("name"), tmpPath));
        rc = loadCategory(&subelement, tmpPath);
      }
      else
      {
        LOG_TRACE(QString("param item %1 is found in %2").arg(subelement.nodeName().toLower(), tmpPath));
        rc = loadParam(&subelement, tmpPath);
      }
      if(!rc)
      {
        LOG_WARNING("loading error");
        ret = false;
        break;
      }
      subelement = subelement.nextSiblingElement();
    }
  }
  while(false);
  return ret;
}

QVariant ConfigManager::getParam(const QString& path, bool& isOk)
{
  QVariant ret;
  isOk = false;
  do
  {
    if(path.isEmpty())
    {
      LOG_WARNING("empty path");
      break;
    }
    if(!params.contains(path))
    {
      LOG_WARNING(QString("param %1 is not found").arg(path));
      break;
    }
    ret = params.value(path);
    isOk = true;
  }
  while(false);
  return ret;
}
