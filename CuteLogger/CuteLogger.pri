message(CuteLogger.pri included in $$PWD)

HEADERS += \
    $$PWD/AbstractAppender.h \
    $$PWD/AbstractStringAppender.h \
    $$PWD/ConsoleAppender.h \
    $$PWD/CuteLogger_global.h \
    $$PWD/FileAppender.h \
    $$PWD/Logger.h \
    $$PWD/OutputDebugAppender.h \
    $$PWD/RollingFileAppender.h \
    $$PWD/cuteloggerinc.h \
    $$PWD/logglobalsetup.h \
    $$PWD/logstarter.h \
    $$PWD/qdebugstub.h \
    $$PWD/syslogappender.h \
    $$PWD/HTTPAppender.h

SOURCES += \
    $$PWD/AbstractAppender.cpp \
    $$PWD/AbstractStringAppender.cpp \
    $$PWD/ConsoleAppender.cpp \
    $$PWD/FileAppender.cpp \
    $$PWD/Logger.cpp \
    $$PWD/OutputDebugAppender.cpp \
    $$PWD/RollingFileAppender.cpp  \
    $$PWD/logglobalsetup.cpp \
    $$PWD/logstarter.cpp \
    $$PWD/qdebugstub.cpp \
    $$PWD/syslogappender.cpp \
    $$PWD/HTTPAppender.cpp
