#ifndef METRICSCOLLECTOR_H
#define METRICSCOLLECTOR_H
#include <QVariantMap>
#include "tools.h"
#include "metricsapikeys.h"
#include "xmutex.h"
namespace Tools
{
  class SimpleMetricsCollector
  {
  public:
    SimpleMetricsCollector();
    virtual ~SimpleMetricsCollector();
    static bool init(const QString& baseDirPath);
    static void add(const QString& name, QVariantMap params = QVariantMap());
    static void add(const QString& name, qint64 value);
    static void add(const QString& name, quint64 value);
    static void add(const QString& name, int value);
    static void add(const QString& name, const QString& value);
  private:
    static SimpleMetricsCollector m_instance;
    void destroy();
    void post(const QString& name, const QString& value);
    XNonRecursiveMutex m_fileMutex;
    QFile m_file;
    QString m_baseDirPath;
    QString m_metricsPath;
    bool m_isEnabled;
  };

#define ADD_METRIC Tools::SimpleMetricsCollector::add
}
#endif // METRICSCOLLECTOR_H
