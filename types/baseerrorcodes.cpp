#include "baseerrorcodes.h"
#include "tools.h"

using namespace Tools;

BaseErrorCodes::BaseErrorCodes(QObject *parent) : QObject(parent)
{

}

QString BaseErrorCodes::toString(BaseErrorCodes::e val)
{
    if (isUniversalReason(val))
    {
        return QEnumToString<BaseErrorCodes::e>(val);
    }
    else
    {
        return QString("extCode %1").arg(int(val));
    }
}

bool BaseErrorCodes::isUniversalReason(int val)
{
    return (val >= e::NoError) && (val < e::EXTENDED_REASON_CODE);
}

bool BaseErrorCodes::isError(BaseErrorCodes::e val)
{
    return !Ranges::inScope<BaseErrorCodes::e>(val,
    {
      e::NoError,
      e::InProgress,
      e::Postponed,
      e::Done,
      e::AsyncDone
    });
}
