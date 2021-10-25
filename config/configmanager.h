#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H
#include <QtXml>
#include <QMap>
#include <QVariant>

class ConfigManager
{
public:
  ConfigManager();
  ~ConfigManager();
  static ConfigManager& getDefaultManager();
  QVariant getParam(const QString& path, bool& isOk);
  bool load(const QString& path);
  bool append(const QString& path);

private:
  static ConfigManager defaultManager;
  bool loadCategory(QDomElement* element, const QString& currentPath = QString());
  bool loadParam(QDomElement* element, const QString& currentPath = QString());
  QMap<QString, QVariant> params;
};

#endif // CONFIGMANAGER_H
