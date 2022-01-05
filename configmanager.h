#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H
#include <QtXml>
#include <QHash>
#include <QVariant>
#include <QObject>

#include "toolsmacro.h"
#include "json.h"
#include "xmutex.h"
#include "configseqentry.h"

namespace Tools
{
namespace RuntimeKeys
{
CONST_LITERAL RTTAG_BIN_PATH("runtime/path/binPath");
CONST_LITERAL RTTAG_LOG_DIR("runtime/path/logDir");
} // namespace RuntimeKeys

class ConfigManager: public QObject
{
    Q_OBJECT
public:
    explicit ConfigManager(QObject *parent = nullptr) noexcept;
    explicit ConfigManager(const ConfigManager&) = delete;
    explicit ConfigManager(const ConfigManager&& other) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    ~ConfigManager() = default;
    ATTR_NODISCARD static ConfigManager& getDefaultManager() noexcept;
    ATTR_NODISCARD QVariant getParam(const QString& path, bool& isOk);
    ATTR_NODISCARD bool getFlag(const QString& path, bool defaultValue);
    ATTR_NODISCARD QVariant getParamOrigin(const QString& path, bool& isOk);
    ATTR_NODISCARD bool getFlagOrigin(const QString& path, bool defaultValue);



    ATTR_NODISCARD bool loadXML(const QString& path);
    ATTR_NODISCARD bool appendXML(const QString& path);

    ATTR_NODISCARD bool loadJson(const QString& path);
    ATTR_NODISCARD bool appendJson(const QString& path, const QString& mountPoint = QString());
    ATTR_NODISCARD bool loadJsonMulti(const QList<ConfigSeqEntry>& queue);
    ATTR_NODISCARD bool appendJsonMulti(const QList<ConfigSeqEntry>& queue, const QString& mountPoint = QString());
    ATTR_NODISCARD bool loadJson(const QtJson::JsonObject object);
    ATTR_NODISCARD bool appendJson(const QtJson::JsonObject object, const QString& mountPoint = QString());

    ATTR_NODISCARD bool contains(const QString& path);
    void setParam(const QString& path, const QVariant value);

    bool dumpPlain(const QString& path);
    QStringList getDump();
    void printDump();
    QString getDumpString();

    INLINE bool loadingIsActive() const {return m_loadingIsActive;}

    uint count() const;
signals:
    /// configuration manager starts loading a configuration tree
    /// multiply update events is expected
    void loadingStarted();

    /// configration tree loading is failed. general tree is reverted
    void loadingFailed();

    /// configration tree loading done
    void loadingDone();

    /// parameter is updated
    void parameterChanged(const QString& name, const QVariant& value);

private:
    QString removeComments(const QString& content) const;
    static ConfigManager defaultManager;
    ATTR_NODISCARD bool loadXMLCategory(QDomElement* element, const QString& mountPoint = QString());
    ATTR_NODISCARD bool loadXMLParam(QDomElement* element, const QString& mountPoint = QString());
    QHash<QString, QVariant> m_params;
    QHash<QString, QVariant> m_runtimeParams;
    QHash<QString, QVariant> m_latestStable;
    XRecursiveMutex m_treeMutex;
    bool m_loadingIsActive;
    uint m_affectedParams;
}; //class ConfigManager
} // namespace Tools
#endif // CONFIGMANAGER_H
