#include <QCoreApplication>
#include "qlongobject.h"

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
printf("%X", (1 << ((sizeof (uint) * 8) - 1)));
  return a.exec();
}
