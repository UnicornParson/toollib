#include "openglhelpertypes.h"
#include "glHeadersInc.h"

using namespace Tools;

uint ShaderType::toNative(uint v)
{
  switch(v)
  {
    case VertexShader:{return GL_VERTEX_SHADER;}
    case FragmentShader:{return GL_FRAGMENT_SHADER;}
    case ComputeShader:{return GL_COMPUTE_SHADER;}
    case TessControlShader:{return GL_TESS_CONTROL_SHADER;}
    case TessEvaluationShader:{return GL_TESS_EVALUATION_SHADER;}
    case GeometryShader:{return GL_GEOMETRY_SHADER;}
    default:{CRASH_POINT(QString("Unknown enum value %1").arg(v)); return 0;}
  }
}
