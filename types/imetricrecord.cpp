#include "imetricrecord.h"

using namespace Tools;

IMetricRecord::IMetricRecord(uint category):
    m_category(category)
{

}

IMetricRecord::~IMetricRecord()
{

}

uint IMetricRecord::category() const
{
    return m_category;
}
