#include "unittestprinterinternal.h"
#include "json.h"
#include "fileutils.h"
#include "tools.h"

using namespace Tools;
using namespace QtJson;

namespace
{
CONST_LITERAL StringLine("-------------------------------------------------------------------------------");
CONST_LITERAL StringEndl(" \n");
CONST_LITERAL JsonSuccess("success");
CONST_LITERAL JsonTime("time");
CONST_LITERAL JsonReason("reason");
CONST_LITERAL JsonPass("pass");
CONST_LITERAL JsonFail("fail");
CONST_LITERAL JsonTestName("name");
CONST_LITERAL HtmlHeaderPath("://testTemplates/htmlHeader.txt");
CONST_LITERAL BodyHeaderPath("://testTemplates/bodyHeader.txt");
CONST_LITERAL HtmlFooterPath("://testTemplates/htmlFooter.txt");
CONST_LITERAL BodyFooterPath("://testTemplates/bodyFooter.txt");

MAYBE_UNUSED_ATTR static QString getHtmlTemplate(const QString& path, bool &ok)
{
    return QString(FileUtils::readFile(path, ok));
}

CONST_LITERAL ColorSuccess("#1abc9c");
CONST_LITERAL ColorFail("#C0392B");

CONST_LITERAL ClassSuccess("itemSuccess");
CONST_LITERAL ClassFail("itemFail");

CONST_LITERAL HtmlRC_PASSED("PASSED");
CONST_LITERAL HtmlRC_FAILED("FAILED");
}

QtJson::JsonObject UnitTestPrinterInternal::toJson(const QHash<QString, UnitTestBase::TestResult>& results) const
{
    QtJson::JsonObject obj;
    uint passed = 0;
    uint failed = 0;
    QtJson::JsonArray failedMessages;
    QtJson::JsonArray passedMessages;
    for (const QString& key: results.keys())
    {
        QtJson::JsonObject entry;
        QString reasonMsg = "";

        UnitTestBase::TestResult result = results.value(key);
        if (!result.bSuccess)
        {
            reasonMsg = QString("Reason: %3. ").arg((result.reason.isEmpty())?"{EMPTY REASON}":result.reason);
        }

        entry.insert(JsonSuccess, (result.bSuccess) ? JsonFail : JsonPass);
        entry.insert(JsonReason, reasonMsg);
        entry.insert(JsonTime, result.strTime);
        entry.insert(JsonReason, reasonMsg);
        entry.insert(JsonTestName, key);

        if (result.bSuccess)
        {
            passedMessages.append(entry);
            ++passed;
        }
        else
        {
            failedMessages.append(entry);
            ++failed;
        }
    }
    obj.insert(JsonSuccess, (failed > 0) ? JsonFail : JsonPass);
    return obj;
}

UnitTestPrinterInternal::UnitTestPrinterInternal()  noexcept
{

}

QString UnitTestPrinterInternal::toPlain(const QHash<QString, UnitTestBase::TestResult>& results) const
{
    QString msg;
    msg += StringLine + StringEndl;
    msg += "TEST RESULTS:"_s + StringEndl;
    msg += StringLine + StringEndl;

    uint passed = 0;
    uint failed = 0;
    QStringList failedMessages;
    QStringList passedMessages;
    for (const QString& key: results.keys())
    {
        QString reasonMsg = "";

        UnitTestBase::TestResult result = results.value(key);
        if (!result.bSuccess)
        {
            reasonMsg = QString("Reason: %3. ").arg((result.reason.isEmpty())?"{EMPTY REASON}":result.reason);
        }
        QString message = QString("%1 - Test %2. %3. Executing time: %4")
                .arg((result.bSuccess) ? RC_PASSED : RC_FAILED,
                     key, result.reason, result.strTime);
        if (result.bSuccess)
        {
            LOG_TRACE(message);
            passedMessages.append(message);
            passed++;
        }
        else
        {
            failedMessages.append(message);
            failed++;
        }
    }
    passedMessages.sort();
    for (const QString& m: passedMessages)
    {
        msg += (m + StringEndl);
    }
    msg += StringLine + StringEndl;
    QString summary = QString("TEST RESULTS: %1, count: %2, passed: %3, failed: %4")
            .arg((failed == 0) ? RC_PASSED : RC_FAILED)
            .arg(results.size())
            .arg(passed)
            .arg(failed);

    if (failed == 0)
    {
        msg += (summary + StringEndl);
    }
    else
    {
        msg += StringLine + StringEndl;
        msg +=("FAILED TESTS:"_s + StringEndl);
        failedMessages.sort();
        for (const QString& m: failedMessages)
        {
            msg += (m + StringEndl);
        }
        msg += StringLine + StringEndl;
        msg += (summary + StringEndl);
    }
    msg += StringLine + StringEndl;
    return msg;
}



QString UnitTestPrinterInternal::toHtml(const QHash<QString, UnitTestBase::TestResult>& results) const
{
    uint passed = 0;
    uint failed = 0;
    QStringList failedMessages;
    QStringList passedMessages;
    bool templatesRc = false;

    const QString HtmlHeader(getHtmlTemplate(HtmlHeaderPath, templatesRc));
    if (!templatesRc)
    {
        LOG_ERROR("cannot load HtmlHeader template");
        return QString();
    }
    const QString BodyHeader(getHtmlTemplate(BodyHeaderPath, templatesRc));
    if (!templatesRc)
    {
        LOG_ERROR("cannot load BodyHeader template");
        return QString();
    }
    const QString HtmlFooter(getHtmlTemplate(HtmlFooterPath, templatesRc));
    if (!templatesRc)
    {
        LOG_ERROR("cannot load HtmlFooter template");
        return QString();
    }
    const QString BodyFooter(getHtmlTemplate(BodyFooterPath, templatesRc));
    if (!templatesRc)
    {
        LOG_ERROR("cannot load BodyFooter template");
        return QString();
    }

    QString msg(HtmlHeader);
    for (const QString& key: results.keys())
    {
        QString reasonMsg = "";

        UnitTestBase::TestResult result = results.value(key);
        if (!result.bSuccess)
        {
            reasonMsg = QString("Reason: %3. ").arg((result.reason.isEmpty())?"{EMPTY REASON}":result.reason);
        }
        QString message = QString("<tr class='%1'><td><b>%2</b></td><td>%3</td><td>%4</td><td>%5</td></tr>")
                .arg((result.bSuccess) ? ClassSuccess : ClassFail)
                .arg((result.bSuccess) ? HtmlRC_PASSED : HtmlRC_FAILED)
                .arg(key, reasonMsg, result.strTime);
        if (result.bSuccess)
        {
            passedMessages.append(message);
            passed++;
        }
        else
        {
            failedMessages.append(message);
            failed++;
        }
    }

    QString summary = QString("TEST RESULTS: %1, count: %2, passed: %3, failed: %4")
            .arg((failed == 0) ? HtmlRC_PASSED : HtmlRC_FAILED)
            .arg(results.size())
            .arg(passed)
            .arg(failed);

    QString HeaderTitle = QString("Unut test results %1").arg(timeStr());
    msg += QString(BodyHeader).arg((failed == 0) ? ColorSuccess : ColorFail).arg(HeaderTitle, summary);
    passedMessages.sort();
    failedMessages.sort();
    for (const QString& m: failedMessages)
    {
        msg += (m + StringEndl);
    }
    for (const QString& m: passedMessages)
    {
        msg += (m + StringEndl);
    }

    msg += BodyFooter + HtmlFooter;

    return msg;
}
