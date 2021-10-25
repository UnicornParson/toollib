#ifndef NAMEDCOLORS_H
#define NAMEDCOLORS_H
#include "vector.h"
namespace NamedColors
{
  #define regcolor3f(name, r, g, b) \
  MAYBE_UNUSED_ATTR Q_DECL_CONSTEXPR Vector3f COLOR_##name (r, g, b); \
  MAYBE_UNUSED_ATTR Q_DECL_CONSTEXPR Vector4f COLOR4_##name = extendV3_V4(COLOR_##name, 1.0f); \
  MAYBE_UNUSED_ATTR const QColor QCOLOR_##name (V3ARGS(COLOR_##name));

#define regcolor3_255(name, r, g, b) regcolor3f(name, TF(r), TF(g), TF(b))

//Vector3f COLOR_WHITE, Vector4f COLOR4_WHITE, QColor QCOLOR_WHITE
  regcolor3f(WHITE, 1.0f, 1.0f, 1.0f)
  regcolor3f(RED, 1.0f, 0.0f, 0.0f)
  regcolor3f(GREEN, 0.0f, 1.0f, 0.0f)
  regcolor3f(CYAN, 0.0f, 1.0f, 1.0f)
  regcolor3f(BLUE, 0.0f, 0.0f, 1.0f)
  regcolor3f(BLACK, 0.0f, 0.0f, 0.0f)


//  Q_DECL_CONSTEXPR Vector3f COLOR_WHITE (1.0f, 1.0f, 1.0f);
//  Q_DECL_CONSTEXPR Vector3f COLOR_RED (1.0f, 0.0f, 0.0f);
//  Q_DECL_CONSTEXPR Vector3f COLOR_GREEN (0.0f, 1.0f, 0.0f);
//  Q_DECL_CONSTEXPR Vector3f COLOR_CYAN (0.0f, 1.0f, 1.0f);
//  Q_DECL_CONSTEXPR Vector3f COLOR_BLUE (0.0f, 0.0f, 1.0f);
//  Q_DECL_CONSTEXPR Vector3f COLOR_BLACK (0.0f, 0.0f, 0.0f);

//  Q_DECL_CONSTEXPR Vector4f COLOR4_WHITE = extendV3_V4(COLOR_WHITE, 1.0f);
//  Q_DECL_CONSTEXPR Vector4f COLOR4_RED = extendV3_V4(COLOR_RED, 1.0f);
//  Q_DECL_CONSTEXPR Vector4f COLOR4_GREEN = extendV3_V4(COLOR_GREEN, 1.0f);
//  Q_DECL_CONSTEXPR Vector4f COLOR4_CYAN = extendV3_V4(COLOR_CYAN, 1.0f);
//  Q_DECL_CONSTEXPR Vector4f COLOR4_BLUE = extendV3_V4(COLOR_BLUE, 1.0f);
//  Q_DECL_CONSTEXPR Vector4f COLOR4_BLACK = extendV3_V4(COLOR_BLACK, 1.0f);

//  const QColor QCOLOR_WHITE (255, 255, 255);
//  const QColor QCOLOR_BLACK (0, 0, 0);
//  const QColor QCOLOR_RED (255, 0, 255);
//  const QColor QCOLOR_GREEN (0, 255, 0);
//  const QColor QCOLOR_CYAN (0, 255, 255);
//  const QColor QCOLOR_BLUE (0, 0, 255);

  //flat UI US from https://flatuicolors.com/palette/us

  regcolor3_255(GRAY0, 223, 230, 233)
  regcolor3_255(GRAY1, 178, 190, 195)
  regcolor3_255(GRAY2, 99, 110, 114)
  regcolor3_255(GRAY3, 45, 52, 54)
  regcolor3_255(GRAY4, 5, 12, 14)


//  Q_DECL_CONSTEXPR Vector3f COLOR_GRAY0 (TF(223), TF(230), TF(233));
//  Q_DECL_CONSTEXPR Vector3f COLOR_GRAY1 (TF(178), TF(190), TF(195));
//  Q_DECL_CONSTEXPR Vector3f COLOR_GRAY2 (TF(99), TF(110), TF(114));
//  Q_DECL_CONSTEXPR Vector3f COLOR_GRAY3 (TF(45), TF(52), TF(54));
//  Q_DECL_CONSTEXPR Vector3f COLOR_GRAY4 (TF(5), TF(12), TF(14));

//  Q_DECL_CONSTEXPR Vector4f COLOR4_GRAY0 = extendV3_V4(COLOR_GRAY0, 1.0f);
//  Q_DECL_CONSTEXPR Vector4f COLOR4_GRAY1 = extendV3_V4(COLOR_GRAY1, 1.0f);
//  Q_DECL_CONSTEXPR Vector4f COLOR4_GRAY2 = extendV3_V4(COLOR_GRAY2, 1.0f);
//  Q_DECL_CONSTEXPR Vector4f COLOR4_GRAY3 = extendV3_V4(COLOR_GRAY3, 1.0f);
//  Q_DECL_CONSTEXPR Vector4f COLOR4_GRAY4 = extendV3_V4(COLOR_GRAY4, 1.0f);

//  const QColor QCOLORGRAY0 (V3ARGS(COLOR_GRAY0));
//  const QColor QCOLORGRAY1 (V3ARGS(COLOR_GRAY1));
//  const QColor QCOLORGRAY2 (V3ARGS(COLOR_GRAY2));
//  const QColor QCOLORGRAY3 (V3ARGS(COLOR_GRAY3));
//  const QColor QCOLORGRAY4 (V3ARGS(COLOR_GRAY4));

#undef regcolor3f
#undef regcolor3_255
}

#endif // NAMEDCOLORS_H
