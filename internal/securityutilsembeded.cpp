#include "securityutilsembeded.h"
#include <QUuid>
#include <QDir>
#include <QString>
#include <QByteArray>
#include <QTime>
#include <QCryptographicHash>
#include "tools.h"
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <cstring>
#include <cstdio>
#include <stdlib.h>
#include <errno.h>
#include <ctime>
#include "qaesencryption.h"

#define AES_VERSION QAESEncryption::AES_256
#define AES_MODE QAESEncryption::OFB
#define AES_PADDING QAESEncryption::ISO
#define HASH_ALGORITHM QCryptographicHash::Sha3_512
#define AES_ROUND_COUNT 5
#define AES_IV_SIZE 16

#ifndef SECURITY_UTILS_NO_EXCEPT
#define BREAK_IF_NO_EXCEPT break
#define RETURN_IF_NO_EXCEPT(rc) return rc
#else
#define BREAK_IF_NO_EXCEPT
#define RETURN_IF_NO_EXCEPT(rc)
#endif

using namespace std;
using namespace Tools;

SecurityUtilsEmbeded SecurityUtilsEmbeded::instance;
bool SecurityUtilsEmbeded::init()
{
  bool bRet = false;
  instance.bSelfTestInProgress = true;
  bRet = selfTest();
  instance.bSelfTestInProgress = false;
  instance.bInitCompleted = bRet;
  return bRet;
}

void SecurityUtilsEmbeded::LogError(const QString& error)
{
  instance.m_sLastError = error;
  LOG_ERROR(error);
#ifndef SECURITY_UTILS_NO_EXCEPT
  THROW_TOOL_LIB_EXCEPTION(error);
#endif
}

SecurityUtilsEmbeded::SecurityUtilsEmbeded():
  mRandomEngine(static_cast<quint32>(time(nullptr))),
  m_sLastError(""),
  bInitCompleted(false),
  bSelfTestInProgress(false)
{

}

QByteArray SecurityUtilsEmbeded::Base64Encode(const QByteArray& in)
{
  return in.toBase64();
}

QString SecurityUtilsEmbeded::Base64Encode(const QString& in)
{
  const QByteArray baIn = in.toUtf8();
  return QString(Base64Encode(baIn));
}

QByteArray SecurityUtilsEmbeded::Base64Decode(const QByteArray& in)
{
  return in.fromBase64(in);
}

QString SecurityUtilsEmbeded::Base64Decode(const QString& in)
{
  const QByteArray baIn = in.toUtf8();
  return QString(Base64Decode(baIn));
}


QString SecurityUtilsEmbeded::getUUIDString()
{
  return QUuid().toString();
}

QByteArray SecurityUtilsEmbeded::getUUIDBytes()
{
  return QUuid().toByteArray();
}

QByteArray SecurityUtilsEmbeded::AESEncrypt(const QByteArray& in, const QByteArray& key, AESSalt* pSalt)
{
  QByteArray ret;
  if (in.isEmpty() || key.isEmpty())
  {
    LogError("empty param");
    RETURN_IF_NO_EXCEPT(ret);
  }
  bool bSaltRc = CreateAESSalt(pSalt);
  if (!bSaltRc)
  {
    LogError("cannot create salt");
    RETURN_IF_NO_EXCEPT(ret);
  }
  QByteArray iv(reinterpret_cast<char*>(pSalt->iv), AES_IV_SIZE);
  QAESEncryption aesEngine(AES_VERSION, AES_MODE, AES_PADDING);
  ret = aesEngine.encode(in, key, iv);
  if (ret.isEmpty())
  {
    LogError("cannot encrypt with embeded tool. output is empty");
  }
  return ret;
}

QByteArray SecurityUtilsEmbeded::AESDecrypt(const QByteArray& in, const QByteArray &key, AESSalt* pSalt)
{
  QByteArray ret;
  if (in.isEmpty() || key.isEmpty() || (pSalt == nullptr))
  {
    LogError("empty param");
    RETURN_IF_NO_EXCEPT(ret);
  }
  QByteArray iv(reinterpret_cast<char*>(pSalt->iv), AES_IV_SIZE);
  QAESEncryption aesEngine(AES_VERSION, AES_MODE, AES_PADDING);
  ret = aesEngine.decode(in, key, iv);
  if (ret.isEmpty())
  {
    LogError("cannot encrypt with embeded tool. output is empty");
  }
  return ret;
}

QString SecurityUtilsEmbeded::SaltString(const size_t len)
{
  return QString(RandBytes(len / 2).toHex());
}

bool SecurityUtilsEmbeded::CreateAESSalt(AESSalt* pSalt)
{
  bool bRet = false;
  do
  {
    if (pSalt == nullptr)
    {
      LogError("null salt");
      BREAK_IF_NO_EXCEPT;
    }
    int s = RandBytes(pSalt->iv, size_t(AES_IV_SIZE));
    if (s != AES_IV_SIZE)
    {
      LogError(QString("incorrect result size. expected: %1 actual: %2").arg(s).arg(AES_IV_SIZE));
      BREAK_IF_NO_EXCEPT;
    }
    bRet = true;
  }
  while(false);
  return bRet;
}

QByteArray SecurityUtilsEmbeded::RandBytes(const size_t count)
{
  unsigned char* buf = new unsigned char[count];
  memset(buf, 0, count);
  int rc = RandBytes(buf, count);
  if (size_t(rc) != count)
  {
    delete[] buf;
    return QByteArray();
  }
  QByteArray ret(reinterpret_cast<char*>(buf), count);
  delete[] buf;
  return ret;
}

int SecurityUtilsEmbeded::RandBytes(unsigned char* ptr, size_t count)
{

  if (!isInitiated())
  {
    LogError("Usage before init");
    RETURN_IF_NO_EXCEPT(-2);
  }
  if (ptr == nullptr || count == 0)
  {
    LogError("invalid arguments");
    RETURN_IF_NO_EXCEPT(-2);
  }
  secureErase(ptr, count);

  for (size_t i = 0; i < count; ++i)
  {
    ptr[i] = static_cast<unsigned char>(SecurityUtilsEmbeded::rand(0, 0xff));
  }
  return static_cast<int>(count);
}

bool SecurityUtilsEmbeded::selfTest()
{
  bool bRet = false;
  do
  {
    LOG_DEBUG("Start self-test");
    if (!isInitiated())
    {
      LogError("Usage before init");
      BREAK_IF_NO_EXCEPT;
    }
    QString suuid = getUUIDString();
    LOG_DEBUG(" - check UUID string");
    if (suuid.isEmpty())
    {
      LOG_WARNING("SecurityUtils::selfTest() empty UUID string");
      break;
    }
    QByteArray buuid = getUUIDBytes();
    if (buuid.isEmpty())
    {
      LogError("SecurityUtils::selfTest() empty UUID byte array");
      BREAK_IF_NO_EXCEPT;
    }
    LOG_DEBUG(" - check Base64Encode");
    QString base64Str = Base64Encode(suuid);
    if (base64Str.isEmpty())
    {
      LogError("SecurityUtils::selfTest() empty base 64 string");
      BREAK_IF_NO_EXCEPT;
    }

    QByteArray base64Array = Base64Encode(buuid);
    if (base64Array.isEmpty())
    {
      LogError("SecurityUtils::selfTest() empty base 64 byte array");
      BREAK_IF_NO_EXCEPT;
    }
    LOG_DEBUG(" - check Base64Decode");
    if (suuid != Base64Decode(base64Str))
    {
      LogError("SecurityUtils::selfTest() base 64 encode != decode for strings");
      BREAK_IF_NO_EXCEPT;
    }

    if (buuid != Base64Decode(base64Array))
    {
      LogError("SecurityUtils::selfTest() base 64 encode != decode for byte array");
      BREAK_IF_NO_EXCEPT;
    }

    LOG_DEBUG(" - check random");
    const size_t randCount = 2048;
    QByteArray randBuf = RandBytes(randCount);
    if (randBuf.isEmpty() || randBuf.size() != randCount)
    {
      LogError(QString().asprintf("SecurityUtils::selfTest() cannot generate rand buffer with size %lld. real size: %lld",
                                 randCount, randBuf.size()));
      BREAK_IF_NO_EXCEPT;
    }

    LOG_DEBUG(" - check erase");
    const size_t bufSize = 2048;
    unsigned char rbuf[bufSize];
    RandBytes(rbuf, bufSize);
    secureErase(rbuf, bufSize);
    for (size_t i = 0; i < bufSize; ++i)
    {
      if (rbuf[i] != 0)
      {
        LogError("erase error");
        BREAK_IF_NO_EXCEPT;
      }
    }

    LOG_DEBUG(" - check hash");
    QByteArray hash = getHash(randBuf);
    if (hash.isEmpty())
    {
      LogError("SecurityUtils::selfTest() cannot generate hash");
      BREAK_IF_NO_EXCEPT;
    }

    const int HashIterationCount = 10;
    bRet = true;
    for (int i = 0; i < HashIterationCount; ++i)
    {
      QByteArray tmpHash = getHash(randBuf);
      if (tmpHash != hash)
      {
        bRet = false;
        LogError("SecurityUtils::selfTest() hashes does not math");
        BREAK_IF_NO_EXCEPT;
      }
    }

    QHash<QString, QString> hashCheckData;
    hashCheckData.insert("o3ETugJW8m3qmnPzYrZT", "1cad0df2d52d79a8631a97f27af7ab3792a52b5605d12cc793680da3be05f0268a2cb3690e42306ac804562456d061e40107a2159f77dbe1348a067efb7eb599");
    hashCheckData.insert("1ji5QuETpnlNOQcuYSTr", "463c592ece43864480c490e3672b795851d4b5ec0bfba284fb6fa380047cecb6e9477bc61a8aab44266e7771d8bb1aed6f9275a21270888e1a05034f7cafd397");
    hashCheckData.insert("lPuRpmkK0OVoxdn3UBVe", "5509a65b52f4e326c7420ed44f3199dc9d17c52dc936c1a86ac5eb44cd78488bd3c43d18db881b2bab3f6bac43d94100e3a303b42fc561203e2e9072eac001e9");
    hashCheckData.insert("ys180JamUCTdjNRtKNCr", "858f9f80dd9ba5b5c3b3d5e845f0b8f5b8737ecdc4ce6c5d736b49d18a3c83083f6f40a438e3b116a864d98f9cd5ff7fe87644a19f5e2be35af748bb8f31fd99");
    hashCheckData.insert("GLt4nEN55lD1xy42LD2U", "162aade658aefa89196008be42d0f2fc6b7f5247f340aa0b257a01ebe5391636405965a3a886d167af7e3093f506c46a21954fc3fd654edc1e7ca80bd94c5872");
    hashCheckData.insert("5jm33GidL9wv2vffbmje", "6c5cccb572f05342b2e6afa3e90013f337d52edb2ca9783418fd2429857bea91596efdd44c77f28bc531b42b6ce34d83c86a52cc9aa952adbb5c9a96dc97ddef");
    hashCheckData.insert("9cEAwdO4QkfotvzkTqRW", "d8c8de83ab11baf884e41c42e97892511bceff2c0755d1927c92ab689f52b3f9a4f4b650c56b1565ecf0c484751b4f52fef49427f63e6d0d8e61bb89bf4cabae");
    hashCheckData.insert("SImhoLTJS9Kd6IjDN7wK", "e4aa8a57c593d8eeab4c0b3b95da1194162a0ca279a8a93f39b27371ffdb168742973c3b23430f5a6403ade8afe5470952521ebe4a6a6bf9135fefd28964554c");
    hashCheckData.insert("Y5vOU7JxusQUYZ12rNUN", "6523cd7ca249fda07632ffe209566a35f6b182e0bbcec5903c8259bef26386540516a71355d341aab5e45d8b3ea95142f775cfad49b7e67d45b2faa9e7f59fab");
    hashCheckData.insert("Q0wMkpIkRs8MMPI8vARx", "da8b35911275ff12c01dae32c75f73a9c6d64a36d59fb13832fd1ba02ea0881c5e9f2e6247450238fe04dab6c60e2c079577913149bced306d2b48952a0a685b");

    bool checkHash = true;
    for (const QString& src: hashCheckData.keys())
    {
      QString tmpHash = QString(getHash(src.toLatin1()));
      if (tmpHash.isEmpty())
      {
        checkHash = false;
        LogError("empty Hash");
        BREAK_IF_NO_EXCEPT;
      }
      if (tmpHash != hashCheckData.value(src, QString()))
      {
        checkHash = false;
        LogError(QString("incorrect hash result. src: %1 expected: %2, actual: %3").arg(src, hashCheckData.value(src, QString()), tmpHash));
        BREAK_IF_NO_EXCEPT;
      }
    }

    if (!checkHash)
    {
      LogError("hash test failed");
      BREAK_IF_NO_EXCEPT;
    }

    LOG_DEBUG(" - check AES");
    bool checkAes = true;
    const int nAesTestIterationCount = 10;
    const int nAesTestKeySize = 2048;
    const int nAesTestDataSize = 1024;
    for (int i = 0; i < nAesTestIterationCount; ++i)
    {
      QByteArray aesKey(RandBytes(nAesTestKeySize));
      QByteArray aesSrc(RandBytes(nAesTestDataSize));
      if (aesKey.isEmpty() || aesSrc.isEmpty())
      {
        checkAes = false;
        LogError("key or src");
        BREAK_IF_NO_EXCEPT;
      }
      AESSalt salt;
      QByteArray coded = AESEncrypt(aesSrc, aesKey, &salt);
      if (coded.isEmpty())
      {
        checkAes = false;
        LogError("empty encoded data");
        BREAK_IF_NO_EXCEPT;
      }
      QByteArray aesDst = AESDecrypt(coded, aesKey, &salt);
      if (aesDst.isEmpty())
      {
        checkAes = false;
        LogError("empty decoded data");
        BREAK_IF_NO_EXCEPT;
      }
      if (aesDst != aesSrc)
      {
        checkAes = false;
        LogError("src and dst does not match");
        BREAK_IF_NO_EXCEPT;
      }
    }

    if (!checkAes)
    {
      LogError("AES test failed");
      BREAK_IF_NO_EXCEPT;
    }

  }
  while(false);
  if (bRet)
  {
    LOG_INFO("security self test completed");
  }
  else
  {
    LogError("security self test FAILED");
  }
  return bRet;
}

//disable optimisation
#ifdef _MSC_VER
#pragma optimize( "", off )
#else //mingw compilation
#pragma GCC push_options
#pragma GCC optimize ("no-elide-constructors")
#endif
void SecurityUtilsEmbeded::secureErase(char* ptr, size_t size)
{
  volatile char* p = ptr;
  for (size_t i = 0; i < size; ++i)
  {
    p[i] = 0;
  }
}

void SecurityUtilsEmbeded::secureErase(void* ptr, size_t size)
{
  secureErase(reinterpret_cast<char*>(ptr), size);
}

#ifdef _MSC_VER
#pragma optimize( "", on )
#else
#pragma GCC pop_options
#endif

QByteArray SecurityUtilsEmbeded::getSignedFingerprint(const QByteArray& in)
{
  Q_UNUSED(in)
  THROW_TOOL_LIB_UNSUPPORTED_EXCEPTION;
#if 0
  QByteArray ret;
  do
  {
    // TODO: PGP sign
    ret = getHash(in);
  }
  while(false);
  return ret;
#endif
}

bool SecurityUtilsEmbeded::verifySignedFingerprint(const QByteArray& in)
{
  UNUSED(in)
  THROW_TOOL_LIB_UNSUPPORTED_EXCEPTION;
#if 0
  bool bRet = false;
  do
  {
    return true;
    // TODO: check PGP sign
  }
  while(false);
  return bRet;
#endif
}

QByteArray SecurityUtilsEmbeded::getHash(const QByteArray& in)
{
  return QCryptographicHash::hash(in, HASH_ALGORITHM).toHex();
}

bool SecurityUtilsEmbeded::isInitiated()
{
  return instance.bInitCompleted || instance.bSelfTestInProgress;
}

quint64 SecurityUtilsEmbeded::rand64()
{
  return instance.mRandomEngine.generate64();
}

int SecurityUtilsEmbeded::rand()
{
  return int(instance.mRandomEngine.generate());
}

int SecurityUtilsEmbeded::rand(int min, int max)
{
  const int realMin = qMin<int>(min, max);
  const int realMax = qMax<int>(min, max);
  int rc = (qAbs<int>(rand()) % (realMax - realMin)) + realMin;
  if ((rc < realMin) || (rc > realMax))
  {
    QString errorMsg = QString("rand error! result is out of bounds min: %1, max: %2, result: %3")
        .arg(realMin)
        .arg(realMax)
        .arg(rc);
    LOG_ERROR(errorMsg);
    THROW_TOOL_LIB_EXCEPTION(errorMsg);
  }
  return rc;
}
