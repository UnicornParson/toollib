#include "dataformatinfo.h"
#include "toolsmacro.h"

using namespace Tools;
#ifdef FEATURE_DATAFORMATS_INFO
namespace JsonKeys
{
CONST_LITERAL TypeKey("type");
CONST_LITERAL TextKey("text");
}
QList<DataFormatInfo*> *DataFormatInfo::m_formats = nullptr;
DataFormatInfo::DataFormatInfo(const QString& json, const QString& formatName)
{
    m_name = formatName;
    bool loadRc = loadData(json);
    Q_ASSERT_X(loadRc, __HERE__, "cannot init format info");
    if(m_formats == nullptr)
    {
        m_formats = new QList<DataFormatInfo*>();
    }
        m_formats->append(this);
}

QMap<QString, DataFormatInfo::FieldInfo_t> DataFormatInfo::fields() const
{
    return m_formatData;
}

bool DataFormatInfo::valid() const
{
    return !m_formatData.isEmpty();
}

QString DataFormatInfo::printable() const
{
    QString msg = "Data format "_s + m_name + "\n";
    for(const QString& field: m_formatData.keys())
    {
        const FieldInfo_t& i = m_formatData[field];
        msg += QString(" - %1 %2 - %3 \n").arg(i.first, field, i.second);
    }
    msg += "\n";
    return msg;
}

QString DataFormatInfo::name() const
{
    return m_name;
}

bool DataFormatInfo::loadData(const QString& json)
{
    bool bRet = false;
    do
    {
        m_formatData.empty();
        QtJson::JsonObject obj = QtJson::parse(json).toMap();
        if(obj.isEmpty())
        {
            Q_ASSERT_X(false, m_name.toLatin1(), QString("object is empty. data: %1").arg(json).toLatin1());
            break;
        }
        bRet = true;
        for(const QString& pname: obj.keys())
        {
            QtJson::JsonObject pval = obj.value(pname).toMap();
            if(pval.contains(JsonKeys::TypeKey) && pval.contains(JsonKeys::TextKey))
            {
                FieldInfo_t i(pval.value(JsonKeys::TypeKey).toString(), pval.value(JsonKeys::TextKey).toString());
                m_formatData.insert(pname, i);
            }
            else
            {
                Q_ASSERT_X(false, m_name.toLatin1(), QString("invalid format %1").arg(obj.value(pname).toString()).toLatin1());
                bRet = false;
                break;
            }
        }
    }
    while(false);
    if(!bRet)
    {
        Q_ASSERT_X(false, m_name.toLatin1(), "loading error");
        m_formatData.empty();
    }
    return bRet;
}

QString DataFormatInfo::printableAll()
{
    QString msg("formats: [\n");
    CONST_LITERAL line("\n-------------------------\n");
    if(m_formats == nullptr){return QString();}
    for(DataFormatInfo* ptr: *m_formats)
    {
        if(ptr != nullptr)
        {
            msg += line + ptr->printable() + "\n";
        }
    }
    msg += "\n]\n";
    return msg;
}
#endif // FEATURE_DATAFORMATS_INFO
