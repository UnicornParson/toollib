#ifndef BASEDIRS_H
#define BASEDIRS_H
#include <QString>
#include <QHash>
#include <QObject>
namespace Tools
{
class BaseDirs: public QObject
{
    Q_OBJECT
    Q_ENUMS(eDirTag)
public:
    enum eDirTag
    {
        APP_NAME,
        IMG_HOME,
        RC_HOME,
        LOG_HOME,
        APP_FULL_PATH
    };
    Q_ENUM(eDirTag)

    static BaseDirs& instance();
  void setExePath(const QString& path);

  static QString getSChome();
  static QString getRChome();
  static QString getIMGhome();
  static QString getLogDir();

private:
  BaseDirs();
  QHash<QString, QString> params;

}; // class BaseDirs
} // namespace Tools
#endif // BASEDIRS_H
