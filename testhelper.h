#ifndef TESTHELPER_H
#define TESTHELPER_H
#include "tools.h"
#include "xmutex.h"

class TestHelper
{
public:
    TestHelper();
    virtual ~TestHelper();
    double getRandDouble();
    int getRandInt();
    unsigned int getRandUInt();
    quint64 getRandUInt64();
    QString getUnicRandomStr(int size = -1 /* unlimited */);
    QString getShortUnicRandomStr();
    QtJson::JsonObject generateJSONObject(int depth = 0);
    QString camelizeStrRegister(const QString& in);
    quint64 counter();

private:
    Tools::XNonRecursiveMutex m_counterMutex;
};
#endif // TESTHELPER_H
