#ifndef OPENGLHELPERTYPES_H
#define OPENGLHELPERTYPES_H
#include <QObject>
#include "tools.h"

namespace Tools
{

  class ShaderType: public QObject
  {
    Q_OBJECT
    Q_ENUMS(e)
  public:
    enum e: uint
    {
      // native: GL_COMPUTE_SHADER, GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER,
      VertexShader = 0,
      FragmentShader,
      ComputeShader,
      TessControlShader,
      TessEvaluationShader,
      GeometryShader
    };
    Q_ENUM(e)

    static INLINE QString toString(uint v)
    {
      return QEnumToString<e>(e(v));
    }
    static uint toNative(uint v);
  };
  using eShaderType = ShaderType::e;

  class VersionFlag: public QObject
  {
    Q_OBJECT
    Q_ENUMS(e)
  public:
    enum e: uint
    {
      GLEWARBVertexProgramFlag = 0,
      GLEW1_1,
      GLEW1_2,
      GLEW1_2_1,
      GLEW1_3,
      GLEW1_4,
      GLEW1_5,
      GLEW2_0,
      GLEW2_1,
      GLEW3_0,
      GLEW3_1,
      GLEW3_2,
      GLEW3_3,
      GLEW4_0,
      GLEW4_1,
      GLEW4_2,
      GLEW4_3,
      GLEW4_4,
      GLEW4_5,
      GLEW4_6,
      GLEW_versionMax,
      GLEW1_1_andHigher,
      GLEW1_2_andHigher,
      GLEW1_2_1_andHigher,
      GLEW1_3_andHigher,
      GLEW1_4_andHigher,
      GLEW1_5_andHigher,
      GLEW2_0_andHigher,
      GLEW2_1_andHigher,
      GLEW3_0_andHigher,
      GLEW3_1_andHigher,
      GLEW3_2_andHigher,
      GLEW3_3_andHigher,
      GLEW4_0_andHigher,
      GLEW4_1_andHigher,
      GLEW4_2_andHigher,
      GLEW4_3_andHigher,
      GLEW4_4_andHigher,
      GLEW4_5_andHigher,
      GLEW4_6_andHigher,
      GLEW_XX_andHigher_MAX,
      FLAGS_COUNT
    };
    Q_ENUM(e)

    static INLINE QString toString(uint v)
    {
      return QEnumToString<e>(e(v));
    }
  };
  using eVersionFlag = VersionFlag::e;
}
#endif // OPENGLHELPERTYPES_H
