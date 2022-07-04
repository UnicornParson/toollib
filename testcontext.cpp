#include "testcontext.h"

using namespace Tools;

TestContext::TestContext(QObject *parent)  noexcept
    : QObject{parent}
{

}

TestContext::~TestContext()
{

}

bool TestContext::isValid() const
{
    return true;
}

void TestContext::reset()
{

}
