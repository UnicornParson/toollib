#ifndef MOPTIONALTEST_H
#define MOPTIONALTEST_H
#include "unittestbase.h"
namespace Tools
{

class MOptionalTest : public UnitTestBase
{
public:
    MOptionalTest();
    virtual ~MOptionalTest();
    void TestPlanSetup();
    void TestPlanCleanup();
protected:
    bool runImpl();
};
} //namespace Tools
#endif // MOPTIONALTEST_H
