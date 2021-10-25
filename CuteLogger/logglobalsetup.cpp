#include "logglobalsetup.h"

namespace
{
#ifdef ENABLE_INITIAL_CONSOLE_LOGGING
constexpr bool initialLoggingEnabled(true);
#else
constexpr bool initialLoggingEnabled(false);
#endif
}

LogGlobalSetup::LogGlobalSetup() noexcept:
    m_consoleLoggingEnabled(initialLoggingEnabled)
{

}

LogGlobalSetup& LogGlobalSetup::self()
{
    static LogGlobalSetup i;
    return i;
}

bool LogGlobalSetup::consoleLoggingEnabled() const
{
    return m_consoleLoggingEnabled;
}

void LogGlobalSetup::setConsoleLoggingEnabled(bool v)
{
    m_consoleLoggingEnabled = v;
}
