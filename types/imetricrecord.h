#ifndef IMETRICRECORD_H
#define IMETRICRECORD_H

#include <QtGlobal>
#include <QStringList>

namespace Tools
{
class IMetricRecord
{
public:
    explicit IMetricRecord(uint category);
    ~IMetricRecord();
    virtual QStringList GetStringRow() = 0;
    virtual QStringList GetHeader() = 0;
    uint category() const;
protected:
    uint m_category;
}; // class IMetricRecord
} // namespace Tools
#endif // IMETRICRECORD_H
