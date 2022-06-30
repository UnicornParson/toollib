#include "imetricrecord.h"

using namespace Tools;

IMetricRecord::IMetricRecord(uint category) noexcept:
    m_category(category)
{

}

IMetricRecord::~IMetricRecord() noexcept
{

}

uint IMetricRecord::category() const
{
    return m_category;
}
