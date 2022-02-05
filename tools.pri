## TOOLLIB FEATURES
## CONFIG += TOOLLIB_OPENGL TOOLLIB_OPENCL TOOLLIB_MUTEX_TRACE TOOLLIB_NETWORK_TRACE TOOLLIB_PERF_MON
## DEFINES += MUTEX_TRACE NETWORK_TRACE PERF_MON


INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

message("SSMIR: $$QT_MAJOR_VERSION")

greaterThan(QT_MAJOR_VERSION, 5) {
  DEFINES += USE_SEPARATE_MUTEX
  DEFINES += USE_QT6_ENV

  # qregexp is deprecated. need to refactor AbstractStringAppender class
  # https://doc-snapshots.qt.io/qt6-dev/qregexp.html
  QT += core5compat
  #find_package(Qt6 COMPONENTS Core5Compat REQUIRED)
  #target_link_libraries(mytarget PRIVATE Qt6::Core5Compat)
}

CONFIG(DINC_TOOLS_MARKER) {
message("double include in $PWD")
} else {
INC_TOOLS_MARKER = 1
export(INC_TOOLS_MARKER)

DEFINES += BUILD_TARGET='"\\\"$$TARGET\\\""'
DEFINES += BUILD_QMAKE_FLAGS='"\\\"$$QMAKE_CFLAGS\\\""'

CONFIG += DINC_TOOLS_MARKER
CONFIG += c++17

CONFIG(release, debug|release) {
    # Release ptimizations
    QMAKE_CXXFLAGS += -flto -O3
}

QT += network xml core concurrent
CONFIG += NO_OPENSSL_LIB
QMAKE_CXXFLAGS_WARN_ON -= -Wreorder -Wformat= -WsignConversion

include(types/toollibTypes.pri)
include(toollibInternal.pri)

HEADERS += \
  $$PWD/crashtrap.h \
  $$PWD/crashtrapnoop.h
SOURCES += \
  $$PWD/crashtrap.cpp \
  $$PWD/crashtrapnoop.cpp


CONFIG(NO_OPENSSL_LIB) {
  message("build without openssl libs")
  DEFINES += "NO_OPENSSL_LIB"
  HEADERS += $$PWD/qaesencryption.h
  SOURCES += $$PWD/qaesencryption.cpp
} else {
  OPENSSL_PATH=$$PWD/openssl
  INCLUDEPATH += $$OPENSSL_PATH/include

  win32:contains(QMAKE_HOST.arch, x86_64) {
    message("Using x64 arch")
    PRE_TARGETDEPS += $$OPENSSL_PATH/x64/lib/libssl.a $$OPENSSL_PATH/x64/lib/libcrypto.a
    LIBS += -L$${OPENSSL_PATH}/x64/lib -llibssl -llibcrypto -luser32 -lgdi32 -lwsock32 -lws2_32 -lcrypt32 -ladvapi32
  } else {
    message("Using x32 arch")
    PRE_TARGETDEPS += $$OPENSSL_PATH/x32/lib/libssl.a $$OPENSSL_PATH/x32/lib/libcrypto.a
    LIBS += -L$${OPENSSL_PATH}/x32/lib -llibssl -llibcrypto -luser32 -lgdi32 -lwsock32 -lws2_32 -lcrypt32 -ladvapi32 -lgdi32
  }
}

CONFIG(TOOLLIB_OPENGL) {
  message("ToolLib: OpenGL feature: on")
  DEFINES += FEATURE_OPENGL
  LIBS += -lOpengl32 -lglu32
  QT +=  opengl
  include($$PWD/gltools/gl_tools.pri)
}

CONFIG(TOOLLIB_OPENCL) {
  message("ToolLib: OpenCL feature: on")
  DEFINES += FEATURE_OPENCL
  #LIBS += -lOpengl32 -lglu32
  #QT +=  opengl

  #HEADERS += \
  #  $$PWD/openglhelper.h \
  #  $$PWD/openglhelperprivate.h

  #SOURCES += \
  #  $$PWD/openglhelper.cpp \
  #  $$PWD/openglhelperprivate.cpp

}

CONFIG(TOOLLIB_MUTEX_TRACE) {
  DEFINES += MUTEX_TRACE
}

CONFIG(TOOLLIB_BADWAY_TRAP) {
  DEFINES += BADWAY_TRAP_FEATURE
}

CONFIG(TOOLLIB_NETWORK_TRACE) {
  DEFINES += NETWORK_TRACE
}

CONFIG(TOOLLIB_PERF_MON) {
  DEFINES += PERF_MON
}

message(tools env:)
message(OPENSSL_PATH: $${OPENSSL_PATH})
message(PRE_TARGETDEPS: $${PRE_TARGETDEPS})
message(LIBS: $${LIBS})
message(INCLUDEPATH: $${INCLUDEPATH})

include($$PWD/CuteLogger/CuteLogger.pri)
include($$PWD/lzma/lzma.pri)

#selftest
TESTSRC=$$PWD/test
HEADERS += \
  $$TESTSRC/toolstest.h
SOURCES += \
  $$TESTSRC/toolstest.cpp


HEADERS += \
    $$PWD/tools.h \
    $$PWD/toolsmacro.h \
    $$PWD/json.h \
    $$PWD/tickcounter.h \
    $$PWD/performancemonitor.h \
    $$PWD/performancesensor.h \
    $$PWD/basedirs.h \
    $$PWD/configmanager.h \
    $$PWD/platformhelper.h \
    $$PWD/interfaces.h \
    $$PWD/unittestbase.h \
    $$PWD/memcontrol.h \
    $$PWD/httpsyncdownloader.h \
    $$PWD/securityutils.h \
    $$PWD/filedownloader.h \
    $$PWD/metricsapikeys.h \
    $$PWD/objectid.h \
    $$PWD/logutils.h \
    $$PWD/xmutex.h \
    $$PWD/sharedxmutex.h \
    $$PWD/asyncmetricscollector.h \
    $$PWD/comparators.h \
    $$PWD/mstimer.h \
    $$PWD/simplemetricscollector.h \
    $$PWD/Updatable.h \
    $$PWD/concurentvalue.h \
    $$PWD/idfactory.h \
    $$PWD/version.h \
    $$PWD/functionscheduler.h\
    $$PWD/buildConstants.h \
    $$PWD/responseobject.h \
    $$PWD/ranges.h

SOURCES += \
    $$PWD/tools.cpp \
    $$PWD/json.cpp \
    $$PWD/tickcounter.cpp \
    $$PWD/performancemonitor.cpp \
    $$PWD/performancesensor.cpp \
    $$PWD/basedirs.cpp \
    $$PWD/configmanager.cpp \
    $$PWD/platformhelper.cpp \
    $$PWD/unittestbase.cpp \
    $$PWD/memcontrol.cpp \
    $$PWD/httpsyncdownloader.cpp \
    $$PWD/securityutils.cpp \
    $$PWD/filedownloader.cpp \
    $$PWD/objectid.cpp \
    $$PWD/xmutex.cpp \
    $$PWD/sharedxmutex.cpp \
    $$PWD/asyncmetricscollector.cpp \
    $$PWD/comparators.cpp \
    $$PWD/mstimer.cpp \
    $$PWD/simplemetricscollector.cpp\
    $$PWD/interfaces.cpp \
    $$PWD/idfactory.cpp \
    $$PWD/version.cpp \
    $$PWD/functionscheduler.cpp\
    $$PWD/buildConstants.cpp


win32: DEFINES+=PLATFORM_WINDOWS
} #else of DINC_TOOLS_MARKER

HEADERS += \
    $$PWD/dataformatinfo.h \
    $$PWD/exceptions/toolsexception.h \
    $$PWD/exceptions/notimplementedexception.h \
    $$PWD/exceptions/toollibexceptions.h \
    $$PWD/fileutils.h \
    $$PWD/literals.h \
    $$PWD/xmutexguard.h

SOURCES += \
    $$PWD/dataformatinfo.cpp \
    $$PWD/exceptions/toolsexception.cpp \
    $$PWD/exceptions/notimplementedexception.cpp \
    $$PWD/fileutils.cpp \
    $$PWD/xmutexguard.cpp

RESOURCES += \
    $$PWD/res/embededData.qrc




