#ifndef SECURITYUTILS_H
#define SECURITYUTILS_H
#include <QByteArray>
#include <QString>

#define SHA384_DIGEST_LENGTH	48
#define SHA512_DIGEST_LENGTH	64

//#define SECURITY_UTILS_NO_EXCEPT
#ifdef NO_OPENSSL_LIB
#include "internal/securityutilsembeded.h"
typedef SecurityUtilsEmbeded SecurityUtils;
#else
//use openssl external lib
class SecurityUtils
{
public:
  SecurityUtils();
  static bool selfTest();
  static bool init();
  static bool isInitiated();
  static QString getLastError();
  static void secureErase(void* ptr, size_t size);
  static void secureErase(char* ptr, size_t size);

  // RAND
  static int rand();
  static int rand(int min, int max);
  static QByteArray RandBytes(const size_t count);
  static int RandBytes(unsigned char* ptr, size_t count);

  // BASE 64
  static QByteArray Base64Encode(const QByteArray& in);
  static QString Base64Encode(const QString& in);
  static QByteArray Base64Decode(const QByteArray& in);
  static QString Base64Decode(const QString& in);

  // QUUID
  static QString getUUIDString();
  static QByteArray getUUIDBytes();

  // AES
  static QByteArray AESEncrypt(const QByteArray& in, const QByteArray& key);
  static QByteArray AESDecrypt(const QByteArray& in, const QByteArray &key);

  //SHA
  static QByteArray getHash(const QByteArray& in);
  //static IHashHelper* createNativeHashHelper();

  // SIGN
  static QByteArray getSignedFingerprint(const QByteArray& in);
  static bool verifySignedFingerprint(const QByteArray& in);
private:

  static SecurityUtils instance;
  bool bInitCompleted;
  bool bSelfTestInProgress;
};
#endif
#endif // SecurityUtils_H
