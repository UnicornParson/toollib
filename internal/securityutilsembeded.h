#ifndef SECURITYUTILSEMBEDED_H
#define SECURITYUTILSEMBEDED_H
#include <QString>
#include <QRandomGenerator>
#define AES_SALT_SIZE 16
class SecurityUtilsEmbeded
{
public:
  struct AESSalt
  {
    unsigned char iv[AES_SALT_SIZE];
  };

  static bool selfTest();
  static bool init();
  static bool isInitiated();
  static QString getLastError() {return instance.m_sLastError;}

  static void secureErase(void* ptr, size_t size);
  static void secureErase(char* ptr, size_t size);

  // RAND
  static int rand();
  static quint64 rand64();
  static int rand(int min, int max);
  static QByteArray RandBytes(const size_t count);
  static int RandBytes(unsigned char* ptr, size_t count);
  static QString SaltString(const size_t len);

  // BASE 64
  static QByteArray Base64Encode(const QByteArray& in);
  static QString Base64Encode(const QString& in);
  static QByteArray Base64Decode(const QByteArray& in);
  static QString Base64Decode(const QString& in);

  // QUUID
  static QString getUUIDString();
  static QByteArray getUUIDBytes();

  // AES
  static QByteArray AESEncrypt(const QByteArray& in, const QByteArray& key, AESSalt* pSalt);
  static QByteArray AESDecrypt(const QByteArray& in, const QByteArray& key, AESSalt* pSalt);
  static bool CreateAESSalt(AESSalt* pSalt);

  //SHA
  static QByteArray getHash(const QByteArray& in);
  //static IHashHelper* createNativeHashHelper();

  // SIGN
  static QByteArray getSignedFingerprint(const QByteArray& in);
  static bool verifySignedFingerprint(const QByteArray& in);
private:
  static void LogError(const QString& error);

  SecurityUtilsEmbeded();
  static SecurityUtilsEmbeded instance;
  QRandomGenerator mRandomEngine;
  QString m_sLastError;
  bool bInitCompleted;
  bool bSelfTestInProgress;
};

#endif // SECURITYUTILSEMBEDED_H
