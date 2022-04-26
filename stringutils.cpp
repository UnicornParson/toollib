#include "stringutils.h"
#include <QRegExp>
#include <QRegularExpression>
#include "toolsmacro.h"

using namespace Tools;

static const QRegularExpression endlineRegexp("\n|\r\n|\r");

QString StringUtils::toString(const QtJson::JsonArray& ja)
{
    return QtJson::serializeStr(ja);
}

QString StringUtils::toString(const QtJson::JsonObject& jo)
{
    return QtJson::serializeStr(jo);
}

QStringList StringUtils::splitLines(const QString& in)
{
    return in.split(endlineRegexp, QT_SKIP_EMPTY_PARTS);
}
