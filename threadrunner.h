#ifndef THREADRUNNER_H
#define THREADRUNNER_H
#include <QThread>
#include "toolsmacro.h"
#include <functional>
#include <exception>

namespace Tools
{
class ThreadRunner: public QThread
{
    Q_OBJECT
public:
    using routine_t = std::function<void()>;
    explicit ThreadRunner(QObject* parent = nullptr);

    void run() override;
    void exec(routine_t func);
    void join();
signals:
    void beforeRun();
    void afterRun();
    void exceptionRaised(const std::exception& e);
protected:
    routine_t m_routine;
}; // class ThreadRunner
}; // namespace Tools
#endif // THREADRUNNER_H
