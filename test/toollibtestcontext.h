#ifndef TOOLLIBTESTCONTEXT_H
#define TOOLLIBTESTCONTEXT_H

#include "testcontext.h"
#include <QObject>
namespace Tools
{
class ToollibTestContext : public TestContext
{
    Q_OBJECT
public:
    explicit ToollibTestContext(QObject *parent = nullptr);
    virtual ~ToollibTestContext();
    virtual bool isValid() const Q_DECL_OVERRIDE;
    virtual void reset() Q_DECL_OVERRIDE;
};
}
#endif // TOOLLIBTESTCONTEXT_H
