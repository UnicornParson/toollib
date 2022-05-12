#ifndef SECURITYUTILS_H
#define SECURITYUTILS_H
#include <QByteArray>
#include <QString>

#define SHA384_DIGEST_LENGTH	48
#define SHA512_DIGEST_LENGTH	64

//#define SECURITY_UTILS_NO_EXCEPT

#include "internal/securityutilsembeded.h"
typedef SecurityUtilsEmbeded SecurityUtils;
#endif // SecurityUtils_H
