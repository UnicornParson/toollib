message(gl_tools in $$PWD)


HEADERS += \
  $$PWD/glHeadersInc.h \
  $$PWD/namedcolors.h \
  $$PWD/openglhelper.h \
  $$PWD/openglhelpertypes.h \
  $$PWD/shader.h \
  $$PWD/vector.h \
  $$PWD/vectorwrapper.h \
  $$PWD/openglhelperprivate.h \
  $$PWD/glToolsInc.h \
  $$PWD/quaternion.h

SOURCES += \
  $$PWD/openglhelper.cpp \
  $$PWD/openglhelperprivate.cpp \
  $$PWD/openglhelpertypes.cpp \
  $$PWD/quaternion.h \
  $$PWD/shader.cpp
