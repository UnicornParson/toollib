#include "securityutils.h"
#ifndef NO_OPENSSL_LIB
#include <QUuid>
#include <QDir>
#include <QString>
#include "tools.h"
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <cstring>
#include <cstdio>
#include <stdlib.h>
#include <errno.h>

#include <openssl/rand.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/ssl.h>
#include <openssl/aes.h>
#include <openssl/sha.h>
#include <openssl/ossl_typ.h>

#define AES_ROUND_COUNT 5
using namespace std;
using namespace Tools;

SecurityUtils SecurityUtils::instance;
bool SecurityUtils::init()
{
  bool bRet = false;
  do
  {
    instance.bSelfTestInProgress = true;
    bRet = selfTest();
    instance.bSelfTestInProgress = false;
  }
  while(false);
  instance.bInitCompleted = bRet;
  return bRet;
}


SecurityUtils::SecurityUtils():
  bInitCompleted(false),
  bSelfTestInProgress(false)
{

}

QString SecurityUtils::getLastError()
{
  char errbuf[256] = {0};
  memset(errbuf, 0, 256);
  ERR_error_string_n(ERR_get_error(), errbuf, sizeof(errbuf));
  errbuf[255] = 0;
  return QString(errbuf);
}


QByteArray SecurityUtils::Base64Encode(const QByteArray& in)
{
  return in.toBase64();
}

QString SecurityUtils::Base64Encode(const QString& in)
{
  QByteArray baIn = in.toUtf8();
  return QString(Base64Encode(baIn));
}

QByteArray SecurityUtils::Base64Decode(const QByteArray& in)
{
  return in.fromBase64(in);
}

QString SecurityUtils::Base64Decode(const QString& in)
{
  QByteArray baIn = in.toUtf8();
  return QString(Base64Decode(baIn));
}


QString SecurityUtils::getUUIDString()
{
  return QUuid().toString();
}

QByteArray SecurityUtils::getUUIDBytes()
{
  return QUuid().toByteArray();
}

QByteArray SecurityUtils::AESEncrypt(const QByteArray& in, const QByteArray& key)
{
  QByteArray ret;
  if (in.isEmpty() || key.isEmpty())
  {
    LOG_WARNING("empty param");
    return ret;
  }
  unsigned char *out = new unsigned char[in.size() * 2];
  memset(out, 0, (in.size() * 2));
  AES_KEY akey;
  const size_t maxKeySize = 4* (4 * (AES_MAXNR + 1));
  size_t keyBufSize = key.size();
  if (keyBufSize > maxKeySize)
  {
    keyBufSize = maxKeySize;
  }
  memcpy((void*)&akey.rd_key, (void*)key.data(), keyBufSize);

  akey.rounds = AES_ROUND_COUNT;
  AES_encrypt((unsigned char *)in.data(), out, &akey);
  ret.append((char*)out);

  return ret;
}

QByteArray SecurityUtils::AESDecrypt(const QByteArray& in, const QByteArray &key)
{
  bool success = false;
  QByteArray ret;
  do
  {
    if (!isInitiated())
    {
      const char* errorStr = "Usage before init";
#ifdef SECURITY_UTILS_NO_EXCEPT
      LOG_ERROR(errorStr)
      break;
#else
      THROW_TOOL_LIB_EXCEPTION(errorStr);
#endif

    }
    if (in.isEmpty() || key.isEmpty())
    {
      LOG_WARNING("Empty Params");
      break;
    }
    unsigned char* tmpSrc = (unsigned char*)in.data();
    unsigned char* tmpKey = (unsigned char*)key.data();

    const size_t srcSize = in.size();
    const size_t outSize = srcSize * 2;
    unsigned char out[outSize];
    memset(out, 0, outSize);

    int outRealSize = 0;//instance.dll_AESDecrypt(tmpSrc, srcSize, tmpKey, out);
    secureErase(tmpSrc, srcSize);
    secureErase(tmpKey, key.size());
    if (outRealSize <= 0)
    {
      LOG_WARNING("decryption failed. reason code: %d", outRealSize);
      break;
    }
    ret.append((char*)out, outRealSize);
    secureErase(out, outSize);
    success = true;
  }
  while(false);
  if (!success)
  {
    ret.clear();
  }
  return ret;
}

QByteArray SecurityUtils::RandBytes(const size_t count)
{
  unsigned char buf[count];
  memset(buf, 0, count);
  int rc = RandBytes(buf, count);
  if (rc != 1)
  {
    return QByteArray();
  }
  QByteArray ret((char*)buf, count);
  return ret;
}

int SecurityUtils::RandBytes(unsigned char* ptr, size_t count)
{

  if (!isInitiated())
  {
    const char* errorStr = "Usage before init";
#ifdef SECURITY_UTILS_NO_EXCEPT
    LOG_ERROR(error)
    return -2;
#else
    THROW_TOOL_LIB_EXCEPTION(errorStr);
#endif
  }
  if (ptr == nullptr || count == 0)
  {
    const char* errorStr = "invalid arguments";
#ifdef SECURITY_UTILS_NO_EXCEPT
    LOG_ERROR(error)
    return -2;
#else
    THROW_TOOL_LIB_EXCEPTION(errorStr);
#endif
  }

  secureErase(ptr, count);
  int ret = RAND_bytes(ptr, count);
  if (ret != 1)
  {
    QString error = QString("cannot get rand bytes. reason: %1").arg(getLastError());
#ifdef SECURITY_UTILS_NO_EXCEPT
    LOG_ERROR(error)
#else
    THROW_TOOL_LIB_EXCEPTION(error);
#endif
  }

  return ret;
}



bool SecurityUtils::selfTest()
{
  bool bRet = false;
  do
  {
    LOG_DEBUG("Start self-test");
    if (!isInitiated())
    {
      const char* errorStr = "Usage before init";
#ifdef SECURITY_UTILS_NO_EXCEPT
      LOG_ERROR(errorStr)
      break;
#else
      THROW_TOOL_LIB_EXCEPTION(errorStr);
#endif
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
      LOG_WARNING("SecurityUtils::selfTest() empty UUID byte array");
      break;
    }
    LOG_DEBUG(" - check Base64Encode");
    QString base64Str = Base64Encode(suuid);
    if (base64Str.isEmpty())
    {
      LOG_WARNING("SecurityUtils::selfTest() empty base 64 string");
      break;
    }

    QByteArray base64Array = Base64Encode(buuid);
    if (base64Array.isEmpty())
    {
      LOG_WARNING("SecurityUtils::selfTest() empty base 64 byte array");
      break;
    }
    LOG_DEBUG(" - check Base64Decode");
    if (suuid != Base64Decode(base64Str))
    {
      LOG_WARNING("SecurityUtils::selfTest() base 64 encode != decode for strings");
      break;
    }

    if (buuid != Base64Decode(base64Array))
    {
      LOG_WARNING("SecurityUtils::selfTest() base 64 encode != decode for byte array");
      break;
    }

    LOG_DEBUG(" - check random");
    const size_t randCount = 2048;
    QByteArray randBuf = RandBytes(randCount);
    if (randBuf.isEmpty() || randBuf.size() != randCount)
    {
      LOG_WARNING("SecurityUtils::selfTest() cannot generate rand buffer with size %d. real size: %d",
                  randCount, randBuf.size());
      break;
    }

    LOG_DEBUG(" - check hash");
    QByteArray hash = getHash(randBuf);
    if (hash.isEmpty())
    {
      LOG_WARNING("SecurityUtils::selfTest() cannot generate hash");
      break;
    }

    const int HashIterationCount = 10;
    bRet = true;
    for (int i = 0; i < HashIterationCount; ++i)
    {
      QByteArray tmpHash = getHash(randBuf);
      if (tmpHash != hash)
      {
        LOG_WARNING("SecurityUtils::selfTest() hashes does not math");
        bRet = false;
        break;
      }
    }
  }
  while(false);
  if (bRet)
  {
    LOG_INFO("security self test completed");
  }
  else
  {
    LOG_ERROR("security self test FAILED");
  }
  return bRet;
}

//disable optimisation
#pragma GCC push_options
#pragma GCC optimize ("no-elide-constructors")
void SecurityUtils::secureErase(char* ptr, size_t size)
{
  volatile char* p = ptr;
  for (size_t i = 0; i < size; ++i)
  {
    *p = 0;
  }
}

void SecurityUtils::secureErase(void* ptr, size_t size)
{
  secureErase((char*)ptr, size);
}
#pragma GCC pop_options

QByteArray SecurityUtils::getSignedFingerprint(const QByteArray& in)
{
  THROW_TOOL_LIB_UNSUPPORTED_EXCEPTION;
  QByteArray ret;
  do
  {
    // TODO: PGP sign
    ret = getHash(in);
  }
  while(false);
  return ret;
}

bool SecurityUtils::verifySignedFingerprint(const QByteArray& in)
{
  THROW_TOOL_LIB_UNSUPPORTED_EXCEPTION;
  bool bRet = false;
  do
  {
    return true;
    // TODO: check PGP sign
  }
  while(false);
  return bRet;
}

QByteArray SecurityUtils::getHash(const QByteArray& in)
{
  QByteArray ret;
  do
  {
    if (!isInitiated())
    {
      const char* errorStr = "Usage before init";
#ifdef SECURITY_UTILS_NO_EXCEPT
      LOG_ERROR(errorStr)
      break;
#else
      THROW_TOOL_LIB_EXCEPTION(errorStr);
#endif

    }
    if (in.isEmpty())
    {
      break;
    }
    unsigned char* dst = (unsigned char*)new char[SHA512_DIGEST_LENGTH];
    memset(dst, 0, SHA512_DIGEST_LENGTH);
    SHA512_CTX sha512;
    SHA512_Init(&sha512);
    SHA512_Update(&sha512, (void*)in.data(), in.size());
    SHA512_Final(dst, &sha512);
    if (dst == nullptr)
    {
      LOG_WARNING("Hash generationg error");
      break;
    }
    ret.append((char*)dst, SHA512_DIGEST_LENGTH);
    delete[] dst;
  }
  while(false);
  return ret;
}


bool SecurityUtils::isInitiated()
{
  return instance.bInitCompleted || instance.bSelfTestInProgress;
}

int SecurityUtils::rand()
{
  int data = 0;
  if (1 != RandBytes((unsigned char*)&data, sizeof(int)))
  {
    QString error = QString("RandBytes error %1").arg(getLastError());
#ifdef SECURITY_UTILS_NO_EXCEPT
    LOG_ERROR(error)
        return rand();
#else
    THROW_TOOL_LIB_EXCEPTION(error);
#endif
  }
  return data;

}

int SecurityUtils::rand(int min, int max)
{
  qint64 data = 0;
  if (1 != RandBytes((unsigned char*)&data, sizeof(qint64)))
  {
    return rand();
  }
  const qint64 realMin = (qint64)qMin(min, max);
  const qint64 realMax = (qint64)qMax(min, max);

  qint64 rc = realMin + (qAbs(data) % (realMax - realMin));
  if ((rc < realMin) || (rc > realMax))
  {
    QString errorMsg = QString("rand error! result is out of bounds min: %1, max: %2, result: %3")
        .arg(realMin)
        .arg(realMax)
        .arg(rc);
    LOG_ERROR(errorMsg);
    THROW_TOOL_LIB_EXCEPTION(errorMsg);
  }
  return (int)rc;
}
#endif //#ifndef NO_OPENSSL_LIB
