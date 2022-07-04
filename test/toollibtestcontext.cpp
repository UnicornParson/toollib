#include "toollibtestcontext.h"
using namespace Tools;
ToollibTestContext::ToollibTestContext(QObject *parent)
    : TestContext{parent}
{

}

ToollibTestContext::~ToollibTestContext()
{

}

bool ToollibTestContext::isValid() const
{
    return true;
}

void ToollibTestContext::reset()
{

}
