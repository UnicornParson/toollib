#include "toollibtestbase.h"
;using namespace Tools;
ToollibTestBase::ToollibTestBase() :
    UnitTestBase(),
    m_context(nullptr)
{

}

ToollibTestBase::~ToollibTestBase()
{

}

Tools::TestContext* ToollibTestBase::getContext()
{
    return static_cast<Tools::TestContext*> (m_context);
}

void ToollibTestBase::setTestContext(Tools::TestContext* context)
{
    // no need to check result
    m_context = dynamic_cast<ToollibTestContext*>(context);
}
