#ifndef STYLER_H
#define STYLER_H
#include <QString>
#include <QVector>
#include <QColor>
#include <QMap>
#include <QDomElement>
#include <QIcon>
namespace Tools
{
  enum StyleState
  {
    STATE_DEFAULT = 0,
    STATE_HOVERED,
    STATE_DISABLED,
    STATE_ALERT,
    STATE_CHECKED
  };

  class Styler
  {
  public:
    virtual ~Styler();
    static void setHomePath(const QString& path);
    static bool loadStyle(const QString& path, const QString& skinFile = QString());
    static QString getStyle(const QString& name, StyleState state = STATE_DEFAULT);
    static QString getImgPath(const QString& name, StyleState state = STATE_DEFAULT);
    static QIcon getIcon(const QString& name, StyleState state = STATE_DEFAULT);
    static QColor getColor(const QString& name);
    static QFont getFont(const QString& name);
  private:
    struct _styleItem
    {
      QString name;
      StyleState state;
      QString body;

      _styleItem():
        name(QString()),
        state(STATE_DEFAULT),
        body(QString())
      {
        ;
      }
    };

    struct _imgItem: public _styleItem
    {
      QIcon icon;

      _imgItem():
        icon(QIcon())
      {
        ;
      }
    };
    bool loadFromFile(const QString& path);
    void parseStyle(const QDomElement& element);
    void parseColor(const QDomElement& element);
    void parseFont(const QDomElement& element);
    void parseImgPath(const QDomElement& element);
    static QString stateToString(StyleState state);
    static StyleState stateFromString(const QString& state);
    QVector<_styleItem> styles;
    QMap<QString, QColor> colors;
    QMap<QString, QFont> fonts;
    QVector<_imgItem> imagePathes;
    Styler();
    QString stylePath;
    QString skinHome;
    static Styler instance;
  };
}
#endif // STYLER_H
