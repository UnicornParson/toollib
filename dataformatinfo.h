#ifndef DATAFORMATINFO_H
#define DATAFORMATINFO_H

#include "tools.h"

namespace Tools
{
#ifdef FEATURE_DATAFORMATS_INFO
class DataFormatInfo
{
public:
    using FieldInfo_t = QPair<QString, QString>;
    DataFormatInfo(const QString& json, const QString& formatName);
    QMap<QString, FieldInfo_t> fields() const;
    bool valid() const;
    QString name() const;
    QString printable() const;

    static QString printableAll();
private:
    QMap<QString, FieldInfo_t> m_formatData;
    QString m_name;
    bool loadData(const QString& json);
    static QList<DataFormatInfo*> *m_formats;
}; // class DataFormatInfo
} // namespace Tools
#define DATA_FORMAT_DECL static const Tools::DataFormatInfo dataFormat;
#define DATA_FORMAT(classname,text) const Tools::DataFormatInfo classname::dataFormat(text, QString(#classname))
#else // FEATURE_DATAFORMATS_INFO
#define DATA_FORMAT_DECL
#define DATA_FORMAT(classname,text)
#endif// FEATURE_DATAFORMATS_INFO
#endif // DATAFORMATINFO_H
