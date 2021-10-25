#include "styler.h"
#include <QtXml>
#include <QList>
#include <QFile>
#include "performancesensor.h"
#include "CuteLogger/cuteloggerinc.h"
#include "tools.h"
#include "basedirs.h"
#include "QFontDatabase"
namespace Tools
{
#define STYLE_SHEET_NAME "style.xml"
  Styler Styler::instance;



  Styler::Styler():
    stylePath(QString())
  {

  }

  Styler::~Styler()
  {

  }

  bool Styler::loadStyle(const QString& path, const QString& skinFile)
  {
    ADD_PERF_SENSOR;
    if(path.isEmpty())
      return false;
    instance.stylePath = path;

    if(instance.stylePath.right(1) != CTools::getPathSeparator())
    {
      instance.stylePath += CTools::getPathSeparator();
    }
    QString skin = (skinFile.isEmpty())?QString(STYLE_SHEET_NAME): skinFile;
    bool loadRet = instance.loadFromFile(QString("%1%2%3").arg(instance.stylePath, CTools::getPathSeparator(), skin));

    DESTROY_PERF_SENSOR;
    return  loadRet;
  }

  bool Styler::loadFromFile(const QString& path)
  {
    ADD_PERF_SENSOR;

    bool ret = false;
    do
    {
      if(path.isEmpty())
      {
        LOG_WARNING("empty path");
        break;
      }
      styles.clear();
      QDomDocument domDoc;
      QString fullPath = CTools::pathNormalize(path);
      QFile file(fullPath);
      bool rc = file.open(QIODevice::ReadOnly);
      LOG_TRACE(QString("try to load style from ").append(fullPath));
      if(!rc)
      {
        LOG_WARNING(QString("loading failed. path: ").append(fullPath));
        break;
      }
      rc = domDoc.setContent(&file);
      if(!rc)
      {
        file.close();
        LOG_WARNING("incorrect xml file. parsing error");
        break;
      }

      QDomNode domRoot= (QDomNode)domDoc.documentElement();
      QDomNode domNode = domRoot.firstChild();
      while(!domNode.isNull())
      {
        if(domNode.isElement())
        {
          QDomElement domElement = domNode.toElement();

          if(domElement.isNull())
          {
            domNode = domNode.nextSibling();
            continue;
          }
          if(domElement.tagName() == "style")
          {
            parseStyle(domElement);
          }
          else if(domElement.tagName() == "color")
          {
            parseColor(domElement);
          }
          else if(domElement.tagName() == "image")
          {
            parseImgPath(domElement);
          }
          else if(domElement.tagName() == "font")
          {
            parseFont(domElement);
          }

        }
        domNode = domNode.nextSibling();
        file.close();
      }
      LOG_TRACE(QString("Loading completed %1 styles; %2 colors had been found; %3 images had been found")
                .arg(styles.size())
                .arg(colors.size())
                .arg(imagePathes.size()));
      ret = true;
    }
    while(false);
    DESTROY_PERF_SENSOR;
    return ret;
  }

  QString Styler::stateToString(StyleState state)
  {

    QString ret;
    switch(state)
    {
      case STATE_DEFAULT:{ret = QString("default");break;}
      case STATE_HOVERED:{ret = QString("hover");break;}
      case STATE_DISABLED:{ret = QString("disable");break;}
      case STATE_ALERT:{ret = QString("alert");break;}
      case STATE_CHECKED:{ret = QString("checked");break;}
    }
    return ret;
  }

  StyleState Styler::stateFromString(const QString& state)
  {
    StyleState ret = STATE_DEFAULT;
    if(state.toLower() == QString("hover"))
    {
      ret = STATE_HOVERED;
    }
    else if(state.toLower() == QString("disable"))
    {
      ret = STATE_DISABLED;
    }
    else if(state.toLower() == QString("alert"))
    {
      ret = STATE_ALERT;
    }
    else if(state.toLower() == QString("checked"))
    {
      ret = STATE_CHECKED;
    }
    return ret;
  }

  QFont Styler::getFont(const QString& name)
  {
    return instance.fonts.value(name);
  }

  QString Styler::getStyle(const QString& name, StyleState state)
  {
    ADD_PERF_SENSOR;
    QString ret;
    QString defaultStyle;
    size_t styleCount = instance.styles.size();
    for(size_t i = 0; i < styleCount; i++)
    {
      _styleItem tmp = instance.styles.at(i);
      if(tmp.name == name)
      {
        if(tmp.state == state)
        {
          ret = tmp.body;
          break;
        }
        if(tmp.state == STATE_DEFAULT || defaultStyle.isEmpty())
        {
          defaultStyle = tmp.body;
        }
      }
    }
    if(ret.isEmpty())
    {
      ret = defaultStyle;
    }

    if(ret.isEmpty())
    {
      LOG_WARNING(QString("Style not found %1/%2").arg(name, stateToString(state)));
    }
    DESTROY_PERF_SENSOR;
    return ret;
  }

  QString Styler::getImgPath(const QString& name, StyleState state)
  {
    ADD_PERF_SENSOR;
    QString ret;
    QString defaultImgPath;
    size_t imagesCount = instance.imagePathes.size();
    for(size_t i = 0; i < imagesCount; i++)
    {
      _styleItem tmp = instance.imagePathes.at(i);
      if(tmp.name == name)
      {
        if(tmp.state == state)
        {
          ret = tmp.body;
          break;
        }
        if(tmp.state == STATE_DEFAULT || defaultImgPath.isEmpty())
        {
          defaultImgPath = tmp.body;
        }
      }
    }
    if(ret.isEmpty())
    {
      ret = defaultImgPath;
    }

    if(ret.isEmpty())
    {
      LOG_WARNING(QString("ImgPath is not found %1/%2").arg(name, stateToString(state)));
    }
    DESTROY_PERF_SENSOR;
    return ret;
  }

  QIcon Styler::getIcon(const QString& name, StyleState state)
  {
    ADD_PERF_SENSOR;
    QIcon ret = QIcon();
    QIcon defaultIcon = QIcon();
    size_t imagesCount = instance.imagePathes.size();
    for(size_t i = 0; i < imagesCount; i++)
    {
      _imgItem tmp = instance.imagePathes.at(i);
      if(tmp.name == name)
      {
        if(tmp.state == state)
        {
          ret = QIcon(tmp.icon);
          break;
        }
        if(tmp.state == STATE_DEFAULT || defaultIcon.isNull())
        {
          defaultIcon = QIcon(tmp.icon);
        }
      }
    }
    if(ret.isNull())
    {
      ret = defaultIcon;
    }

    if(ret.isNull())
    {
      LOG_WARNING(QString("Image is not found %1/%2").arg(name, stateToString(state)));
    }
    DESTROY_PERF_SENSOR;
    return ret;
  }
  void Styler::parseStyle(const QDomElement& element)
  {
    ADD_PERF_SENSOR;
    _styleItem newStyle;
    newStyle.name = element.attribute("name", "");
    newStyle.state = stateFromString(element.attribute("state", "default"));
    newStyle.body = element.text();
    styles.append(newStyle);
    DESTROY_PERF_SENSOR;
  }

  void Styler::parseImgPath(const QDomElement& element)
  {
    ADD_PERF_SENSOR;
    _imgItem newImgItem;
    newImgItem.name = element.attribute("name", "");
    newImgItem.state = stateFromString(element.attribute("state", "default"));
    newImgItem.body = element.text();
    if(false == newImgItem.body.isEmpty())
    {
      QString fullIconPath = CTools::pathNormalize(QString("%2/%3")
                                                   .arg(stylePath,
                                                        newImgItem.body));
      LOG_TRACE(QString("try to load icon from %1").arg(fullIconPath));
      newImgItem.icon = QIcon(fullIconPath);
      if(false == newImgItem.icon.isNull())
      {
        imagePathes.append(newImgItem);
      }
      else
      {
        LOG_WARNING(QString("icon loading error. name: %1/%2").arg(newImgItem.name, stateToString(newImgItem.state)));
      }
    }
    DESTROY_PERF_SENSOR;
  }

  void Styler::parseColor(const QDomElement& element)
  {
    ADD_PERF_SENSOR;
    bool isOk = false;
    int r = element.attribute("r", "0").toInt(&isOk);
    if(!isOk)
      r = 0;

    int g = element.attribute("g", "0").toInt(&isOk);
    if(!isOk)
      g = 0;

    int b = element.attribute("b", "0").toInt(&isOk);
    if(!isOk)
      b = 0;

    int a = element.attribute("a", "255").toInt(&isOk);
    if(!isOk)
      b = 255;

    QColor newColor(r,g,b,a);

    colors.insert(element.attribute("name", ""), newColor);
    DESTROY_PERF_SENSOR;
  }

  void Styler::parseFont(const QDomElement& element)
  {
    ADD_PERF_SENSOR;
    do
    {
      QString fontAlisas = element.attribute("name", "");
      if(fontAlisas.isEmpty())
      {
        LOG_WARNING("empty name");
        break;
      }
      QString fontPath = element.text();
      if(fontPath.isEmpty())
      {
        LOG_WARNING("empty path");
        break;
      }
      QString fullFontPath = CTools::pathNormalize(QString("%2/%3").arg(stylePath,fontPath));
      LOG_TRACE(QString("try to load icon from %1").arg(fullFontPath));
      int fontRc = QFontDatabase::addApplicationFont(fullFontPath);
      if(fontRc == -1)
      {
        LOG_WARNING(QString("font %1 cannot be loaded").arg(fullFontPath));
        break;
      }
      QString family = QFontDatabase::applicationFontFamilies(fontRc).at(0);
      QFont newFont(family);
      fonts.insert(fontAlisas, newFont);
    }
    while(false);
    DESTROY_PERF_SENSOR;
  }

  QColor Styler::getColor(const QString& name)
  {
    return instance.colors.value(name, QColor(0,0,0));
  }
}
