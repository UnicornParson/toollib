#ifndef UNITTESTPRINTERINTERNAL_H
#define UNITTESTPRINTERINTERNAL_H

#include "unittestbase.h"
#include "toolsmacro.h"
#include "json.h"

namespace Tools
{
CONST_LITERAL RC_PASSED("  PASSED  ");
CONST_LITERAL RC_FAILED("> FAILED <");
class UnitTestPrinterInternal
{
public:
    UnitTestPrinterInternal() noexcept;
    QString toPlain(const QHash<QString, UnitTestBase::TestResult>& results) const;
    QtJson::JsonObject toJson(const QHash<QString, UnitTestBase::TestResult>& results) const;
    QString toHtml(const QHash<QString, UnitTestBase::TestResult>& results) const;
}; // class UnitTestPrinterInternal
} // namespace mgenn
#endif // UNITTESTPRINTERINTERNAL_H
