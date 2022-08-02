#ifndef RANGESTEST_H
#define RANGESTEST_H

#include "toollibtestbase.h"
namespace Tools
{
class RangesTest : public ToollibTestBase
{
public:
    RangesTest();
    virtual ~RangesTest();
    void TestPlanSetup();
    void TestPlanCleanup();
protected:
    bool runImpl();
}; // class RangesTest
} // namespace Tools
#endif // RANGESTEST_H
