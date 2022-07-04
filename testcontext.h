#ifndef TESTCONTEXT_H
#define TESTCONTEXT_H

#include <QObject>
namespace Tools
{
class TestContext : public QObject
{
    Q_OBJECT
public:
    explicit TestContext(QObject *parent = nullptr) noexcept;
    virtual ~TestContext();
    virtual bool isValid() const;
    virtual void reset();
signals:

};
}
#endif // TESTCONTEXT_H
