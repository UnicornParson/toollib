#include "stringutils.h"

using namespace Tools;

QString StringUtils::toString(const QtJson::JsonArray& ja)
{
    return QtJson::serializeStr(ja);
}

QString StringUtils::toString(const QtJson::JsonObject& jo)
{
    return QtJson::serializeStr(jo);
}
