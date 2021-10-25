message("LZMA.pri included in $$PWD")

DEFINES += _7ZIP_ST

SOURCES += \
    $$PWD/LzmaDec.c \
    $$PWD/LzmaEnc.c \
    $$PWD/Alloc.c \
    $$PWD/LzFind.c \
    $$PWD/Lzma2Dec.c \
    $$PWD/Lzma2Enc.c \
    $$PWD/lzma.cpp

HEADERS += \
    $$PWD/LzmaDec.h \
    $$PWD/LzmaEnc.h \
    $$PWD/Alloc.h \
    $$PWD/Types.h \
    $$PWD/LzFind.h \
    $$PWD/Lzma2Dec.h \
    $$PWD/Lzma2Enc.h \
    $$PWD/Types.h \
    $$PWD/LzHash.h \
    $$PWD/lzma.h
