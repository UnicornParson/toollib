#include "threadrunner.h"
#include "tools.h"

using namespace Tools;

ThreadRunner::ThreadRunner(QObject *parent)
    :QThread(parent)
{
    m_routine = [](){};
}

void ThreadRunner::exec(routine_t func)
{
    m_routine = func;
    start();
}

void ThreadRunner::join()
{
    while(isRunning())
    {
        mssleep(10);
    }
}

void ThreadRunner::run()
{
    emit beforeRun();
    try
    {
        m_routine();
    }
    catch (const std::exception& e)
    {
        emit exceptionRaised(e);
    }
    emit afterRun();
}
