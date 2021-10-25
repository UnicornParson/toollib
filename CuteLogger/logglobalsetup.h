#ifndef LOGGLOBALSETUP_H
#define LOGGLOBALSETUP_H
#include "toolsmacro.h"

class LogGlobalSetup
{
public:
    LogGlobalSetup() noexcept;
    ~LogGlobalSetup() = default;
    LogGlobalSetup(LogGlobalSetup&) = delete;
    LogGlobalSetup(LogGlobalSetup&&) = delete;
    LogGlobalSetup& operator=(LogGlobalSetup&) = delete;
    static LogGlobalSetup& self();

    bool consoleLoggingEnabled() const;
    void setConsoleLoggingEnabled(bool v);
private:
    bool m_consoleLoggingEnabled;
};

#endif // LOGGLOBALSETUP_H
