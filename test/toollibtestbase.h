#ifndef TOOLLIBTESTBASE_H
#define TOOLLIBTESTBASE_H

#include "unittestbase.h"
#include "toollibtestcontext.h"
namespace Tools
{
class ToollibTestBase : public UnitTestBase
{
public:
    ToollibTestBase();
    virtual ~ToollibTestBase();
    virtual Tools::TestContext* getContext() Q_DECL_OVERRIDE;
    virtual void setTestContext(Tools::TestContext* context) Q_DECL_OVERRIDE;
protected:
    ToollibTestContext *m_context;
};
}
#endif // TOOLLIBTESTBASE_H
