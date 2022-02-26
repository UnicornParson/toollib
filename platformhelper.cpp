#include "platformhelper.h"
#include <QProcess>
#include "CuteLogger/cuteloggerinc.h"
#ifdef PLATFORM_WINDOWS
#include <windows.h>
//#include <VersionHelpers.h>
#else
#include <sys/types.h> /* POSIX */
#include <unistd.h>
#endif
#include <QCryptographicHash>

#ifdef PLATFORM_WINDOWS
#ifndef _WIN32_WINNT_NT4
#define _WIN32_WINNT_NT4      0x0400
#endif
#ifndef _WIN32_WINNT_WIN2K
#define _WIN32_WINNT_WIN2K    0x0500
#endif
#ifndef _WIN32_WINNT_WINXP
#define _WIN32_WINNT_WINXP    0x0501
#endif
#ifndef _WIN32_WINNT_WS03
#define _WIN32_WINNT_WS03     0x0502
#endif
#ifndef _WIN32_WINNT_WIN6
#define _WIN32_WINNT_WIN6     0x0600
#endif
#ifndef _WIN32_WINNT_VISTA
#define _WIN32_WINNT_VISTA    0x0600
#endif
#ifndef _WIN32_WINNT_WS08
#define _WIN32_WINNT_WS08     0x0600
#endif
#ifndef _WIN32_WINNT_LONGHORN
#define _WIN32_WINNT_LONGHORN 0x0600
#endif
#ifndef _WIN32_WINNT_WIN7
#define _WIN32_WINNT_WIN7     0x0601
#endif
#ifndef _WIN32_WINNT_WIN8
#define _WIN32_WINNT_WIN8     0x0602
#endif
#ifndef _WIN32_WINNT_WINBLUE
#define _WIN32_WINNT_WINBLUE  0x0603
#endif
#ifndef _WIN32_WINNT_WIN10
#define _WIN32_WINNT_WIN10    0x0A00
#endif

BOOL IsWinVersionOrGreater(DWORD id, WORD wServicePackMajor)
{
  WORD wMajorVersion = HIBYTE(id);
  WORD wMinorVersion = LOBYTE(id);

  OSVERSIONINFOEXW osvi = { sizeof(osvi), 0, 0, 0, 0,{ 0 }, 0, 0, 0, 0, 0};
  DWORDLONG const dwlConditionMask =
      VerSetConditionMask(
        VerSetConditionMask(
          VerSetConditionMask(0, VER_MAJORVERSION, VER_GREATER_EQUAL),
          VER_MINORVERSION, VER_GREATER_EQUAL),
        VER_SERVICEPACKMAJOR, VER_GREATER_EQUAL);

  osvi.dwMajorVersion = wMajorVersion;
  osvi.dwMinorVersion = wMinorVersion;
  osvi.wServicePackMajor = wServicePackMajor;

  return VerifyVersionInfoW(&osvi, VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR, dwlConditionMask) != FALSE;
}

BOOL IsWindowsXPOrGreater()      {  return IsWinVersionOrGreater(_WIN32_WINNT_WINXP, 0);}
BOOL IsWindowsXPSP1OrGreater()   {  return IsWinVersionOrGreater(_WIN32_WINNT_WINXP, 1);}
BOOL IsWindowsXPSP2OrGreater()   {  return IsWinVersionOrGreater(_WIN32_WINNT_WINXP, 2);}
BOOL IsWindowsXPSP3OrGreater()   {  return IsWinVersionOrGreater(_WIN32_WINNT_WINXP, 3);}
BOOL IsWindowsVistaOrGreater()   {  return IsWinVersionOrGreater(_WIN32_WINNT_VISTA, 0);}
BOOL IsWindowsVistaSP1OrGreater(){  return IsWinVersionOrGreater(_WIN32_WINNT_VISTA, 1);}
BOOL IsWindowsVistaSP2OrGreater(){  return IsWinVersionOrGreater(_WIN32_WINNT_VISTA, 2);}
BOOL IsWindows7OrGreater()       {  return IsWinVersionOrGreater(_WIN32_WINNT_WIN7,  0);}
BOOL IsWindows7SP1OrGreater()    {  return IsWinVersionOrGreater(_WIN32_WINNT_WIN7,  1);}
BOOL IsWindows8OrGreater()       {  return IsWinVersionOrGreater(_WIN32_WINNT_WIN8,  0);}
BOOL IsWindows8Point1OrGreater() {  return IsWinVersionOrGreater(_WIN32_WINNT_WINBLUE, 0); }
BOOL IsWindows10OrGreater()      {  return IsWinVersionOrGreater(_WIN32_WINNT_WIN10, 0); }

BOOL IsWindowsServer()
{
  OSVERSIONINFOEXW osvi = { sizeof(osvi), 0, 0, 0, 0,{ 0 }, 0, 0, 0, VER_NT_WORKSTATION, 0 };
  DWORDLONG const dwlConditionMask = VerSetConditionMask(0, VER_PRODUCT_TYPE, VER_EQUAL);
  return !VerifyVersionInfoW(&osvi, VER_PRODUCT_TYPE, dwlConditionMask);
}
#endif

namespace Tools
{
  quint64 PlatformHelper::getPid()
  {
    quint64 ret;
#ifdef PLATFORM_WINDOWS
    ret = static_cast<quint64>(GetCurrentProcessId());
#else
    ret = (quint64)getpid();
#endif
    return ret;
  }

  QByteArray PlatformHelper::getInstanceIDData()
  {
    QByteArray src;
#ifdef PLATFORM_WINDOWS
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    src.append(QString().asprintf("%llu", static_cast<unsigned long long>(sysinfo.dwActiveProcessorMask)).toUtf8());
    src.append(QString().asprintf("%llu", static_cast<unsigned long long>(sysinfo.dwNumberOfProcessors)).toUtf8());
    src.append(QString().asprintf("%llu", static_cast<unsigned long long>(sysinfo.dwProcessorType)).toUtf8());
    src.append(QString().asprintf("%llu", static_cast<unsigned long long>(sysinfo.dwAllocationGranularity)).toUtf8());
    src.append(QString().asprintf("%llu", static_cast<unsigned long long>(sysinfo.wProcessorLevel)).toUtf8());
    src.append(QString().asprintf("%llu", static_cast<unsigned long long>(sysinfo.wProcessorRevision)).toUtf8());
#else
    QString cpuinfoPath = "/proc/cpuinfo";

    QFile fcpuinfo(cpuinfoPath);
    if (fcpuinfo.open(QFile::ReadOnly))
    {
      src.append(fcpuinfo.readAll());
      fcpuinfo.close();
    }
    else
    {
      LOG_ERROR("cannot read cpu info");
    }
#endif
    src.append(getOSVersion().toUtf8());
    return src;
  }

  QString PlatformHelper::getShortInstanceID()
  {
    QString ret;
    QByteArray src = getInstanceIDData();
    QByteArray hash = QCryptographicHash::hash(src, QCryptographicHash::Sha1);
    ret = QString("%1_%2").arg(QString(hash.toHex())).arg(getPid());
    return ret;
  }

  QString PlatformHelper::getAppInstanceID()
  {
    QString ret;
    QByteArray src = getInstanceIDData();
    QByteArray hash = QCryptographicHash::hash(src, QCryptographicHash::Sha3_512);
    ret = QString("%1:%2").arg(QString(hash.toHex())).arg(getPid());
    return ret;
  }

  int PlatformHelper::getProcessorsCount()
  {
    int ret = 0;
#ifdef PLATFORM_WINDOWS
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    ret = static_cast<int>(sysinfo.dwNumberOfProcessors);
#else
     ret = sysconf(_SC_NPROCESSORS_ONLN);
#endif
    return ret;
  }

  QHash<QString, quint64> PlatformHelper::getFullMemInfo()
  {
    QHash<QString, quint64> ret;
#ifdef PLATFORM_WINDOWS
    ret = getWindowsMeminfo();
#else
    ret = getLinuxMeminfo();
#endif
    return ret;
  }

  quint64 PlatformHelper::getTotalMem()
  {
    return getFullMemInfo().value("MemTotal", 0);
  }

  quint64 PlatformHelper::getFreeMem()
  {
    return getFullMemInfo().value("MemFree", 0);
  }

  quint64 PlatformHelper::getAvailableMem()
  {
    return getFullMemInfo().value("MemAvailable", 0);
  }

  quint64 PlatformHelper::getMemLoad()
  {
    quint64 ret = 0;
    QHash<QString, quint64> memmap = getFullMemInfo();
    if (memmap.contains("MemoryLoad"))
    {
      ret = memmap.value("MemoryLoad", 0);
    }
    else
    {
      ret = static_cast<quint64>(((static_cast<double>(getTotalMem()) / static_cast<double>(getTotalMem())) * 100.0));
    }

    return ret;
  }

  QString PlatformHelper::getOSVersion()
  {
    QString ret;
#ifdef PLATFORM_WINDOWS
    if (IsWindowsXPOrGreater()){ret = "Windows XP";}
    if (IsWindowsXPSP1OrGreater()){ret = "Windows XP SP1";}
    if (IsWindowsXPSP2OrGreater()){ret = "Windows XP SP2";}
    if (IsWindowsXPSP3OrGreater()){ret = "Windows XP SP3";}
    if (IsWindowsVistaOrGreater()){ret = "Windows Vista";}
    if (IsWindowsVistaSP1OrGreater()){ret = "Windows XP SP1";}
    if (IsWindowsVistaSP2OrGreater()){ret = "Windows XP SP2";}
    if (IsWindows7OrGreater()){ret = "Windows 7";}
    if (IsWindows7SP1OrGreater()){ret = "Windows 7 SP1";}
    if (IsWindows8OrGreater()){ret = "Windows 8";}
    if (IsWindows8Point1OrGreater()){ret = "Windows 8 point 1";}
    if (IsWindows10OrGreater()){ret = "Windows 10";}

    if (IsWindowsServer()){ret += " Server";}
#else
    QString versionPath = "/proc/version_signature";
    if (!QFile::exists(versionPath))
    {
      versionPath = "/proc/version";
    }
    QFile fosinfo(versionPath);
    if (fosinfo.open(QFile::ReadOnly))
    {
      QString osversion(fosinfo.readAll());
      ret = osversion;
      fosinfo.close();
    }
    else
    {
      LOG_ERROR("cannot read os info");
    }
#endif
    return ret;
  }

  QHash<QString, quint64> PlatformHelper::getLinuxMeminfo()
  {
    QHash<QString, quint64> ret;
    const QString infopath("/proc/meminfo");
    QFile fmeminfo(infopath);
    if (fmeminfo.open(QFile::ReadOnly))
    {

      while(!fmeminfo.atEnd())
      {
        QString line = QString(fmeminfo.readLine()).trimmed();
#ifdef USE_QT6_ENV
        QStringList sublines = line.split(":", QT_SKIP_EMPTY_PARTS);
#else
        QStringList sublines = line.split(":" ,QString::SkipEmptyParts);
#endif

        if (sublines.size() != 2)
        {
          LOG_WARNING(QString("invalid line %1").arg(line));
          continue;
        }

        //TODO: test on linux
        LOG_DEBUG(QString("found line [%1]---[%2]").arg(sublines[0].trimmed(), sublines[1].trimmed()));
      }
      fmeminfo.close();
    }
    else
    {
      LOG_ERROR("cannot read mem info");
    }
    return ret;

  }

  QHash<QString, quint64> PlatformHelper::getWindowsMeminfo()
  {
    QHash<QString, quint64> ret;
#ifdef PLATFORM_WINDOWS
    MEMORYSTATUSEX memory_status;
    ZeroMemory(&memory_status, sizeof(MEMORYSTATUSEX));
    memory_status.dwLength = sizeof(MEMORYSTATUSEX);
    if (GlobalMemoryStatusEx(&memory_status))
    {
      ret.insert("MemoryLoad", static_cast<quint64>(memory_status.dwMemoryLoad));
      ret.insert("MemTotal", static_cast<quint64>(memory_status.ullTotalVirtual));
      ret.insert("MemAvailable", static_cast<quint64>(memory_status.ullAvailVirtual));
      ret.insert("TotalPhys", static_cast<quint64>(memory_status.ullTotalPhys));
      ret.insert("AvailPhys", static_cast<quint64>(memory_status.ullAvailPhys));
      ret.insert("TotalPageFile", static_cast<quint64>(memory_status.ullTotalPageFile));
      ret.insert("AvailPageFile", static_cast<quint64>(memory_status.ullAvailPageFile));
      ret.insert("AvailExtendedVirtual", static_cast<quint64>(memory_status.ullAvailExtendedVirtual));
    }
    else
    {
      LPTSTR lpMsgBuf;
      DWORD dw = GetLastError();

      FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            dw,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            reinterpret_cast<LPTSTR>(&lpMsgBuf),
            0, nullptr );
      LOG_WARNING(QString("cannot read memory info. reason: %1").arg(QString::fromWCharArray(lpMsgBuf)));
    }
#endif
    return ret;
  }

}
