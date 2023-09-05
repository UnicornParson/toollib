#ifndef VECTORX_H
#define VECTORX_H
#include "quaternion.h"

#ifdef NO_QT_OPENGL
#include <GL/gl.h>
#include <GL/glext.h>
#else
#include <QtGui/QOpenGLFunctions>
#endif

#include <QSize>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QRandomGenerator>
#include <QColor>

#include "tools.h"
//=========================================================================================================================================
// Vector4 template
//=========================================================================================================================================

//#define USE_EMBEDED_GLSETTERS
#define eX d[0]
#define eY d[1]
#define eZ d[2]
#define eW d[3]

template< typename type >
class Vector4
{
public:
  type d[4];
  inline type&  x(){return eX;}
  inline type&  y(){return eY;}
  inline type&  z(){return eZ;}
  inline type&  w(){return eW;}

 Q_DECL_CONSTEXPR inline type cx() const {return eX;}
 Q_DECL_CONSTEXPR inline type cy() const {return eY;}
 Q_DECL_CONSTEXPR inline type cz() const {return eZ;}
 Q_DECL_CONSTEXPR inline type cw() const {return eW;}
 Q_DECL_CONSTEXPR Vector4():Vector4(type(0)) {}
 Q_DECL_CONSTEXPR Vector4(type _x, type _y, type _z, type _w):d{_x, _y, _z, _w}{}
 Q_DECL_CONSTEXPR Vector4(const Vector4<type>& v):d{v.d[0], v.d[1], v.d[2], v.d[3]}{}
 Q_DECL_CONSTEXPR explicit Vector4(type* a):d{a[0], a[1], a[2], a[3]}{}
 Q_DECL_CONSTEXPR explicit Vector4(type v):d{v, v, v, v}{}
 Q_DECL_CONSTEXPR inline void set(type _x, type _y, type _z, type _w) { eX = _x; eY = _y; eZ = _z; eW = _w;}
 Q_DECL_CONSTEXPR inline void set(type val) { eX = val; eY = val; eZ = val; eW = val;}
 Q_DECL_CONSTEXPR inline bool isZero() const {return (eX == 0) && (eY == 0) && (eZ == 0) && (eW == 0);}
 Q_DECL_CONSTEXPR inline bool isOne() const {return (eX == 1) && (eY == 1) && (eZ == 1) && (eW == 0);}


  void Print(bool endl = true) const
  {
    UNUSED(endl)
    LOG_TRACE(toString());
  }

  inline QString toString() const
  {
    return QString("%1; %2; %3; %4")
        .arg(double(cx()))
        .arg(double(cy()))
        .arg(double(cz()))
        .arg(double(cw()));
  }

 Q_DECL_CONSTEXPR operator const type*() const
  {
    return d;
  }

  operator QVector4D() const
  {
    return QVector4D(float(x()),
                     float(y()),
                     float(z()),
                     float(w()));
  }

  Vector4<type>& Normalize()
  {
    const type Length = static_cast<type>(sqrtf(static_cast<float>((eX * eX) + (eY * eY) + (eZ * eZ) + (eW * eW))));
    eX /= Length; eY /= Length; eZ /= Length; eW /= Length;
    return *this;
  }

 Q_DECL_CONSTEXPR inline Vector4<type>& operator+=(const Vector4<type>& r) { eX += r.x(); eY += r.y(); eZ += r.z(); eW += r.w(); return *this; }
 Q_DECL_CONSTEXPR inline Vector4<type>& operator-=(const Vector4<type>& r) { eX -= r.x(); eY -= r.y(); eZ -= r.z(); eW -= r.w(); return *this; }
 Q_DECL_CONSTEXPR inline Vector4<type>& operator+=(type v) { eX += v; eY += v; eZ += v; eW += v; return *this; }
 Q_DECL_CONSTEXPR inline Vector4<type>& operator-=(type v) { eX -= v; eY -= v; eZ -= v; eW -= v; return *this; }
 Q_DECL_CONSTEXPR inline Vector4<type> operator+(const Vector4<type>& r) const { return Vector4<type>(eX + r.x(), eY + r.y(), eZ + r.z(), eW + r.w());}
 Q_DECL_CONSTEXPR inline Vector4<type> operator-(const Vector4<type>& r) const { return Vector4<type>(eX - r.x(), eY - r.y(), eZ - r.z(), eW + r.w());}
 Q_DECL_CONSTEXPR inline size_t mem_size() const {return 4 * sizeof(type);}
};

#define SetVector4AsGlColorF(v) glColor4fv(v.d)
#define SetVector4AsGlColorD(v) glColor4dv(v.d)
#define SetVector4AsGlClearColorD(v) glClearColor(GLfloat(v.cx()), GLfloat(v.cy()), GLfloat(v.cz()), GLfloat(v.cw()))
/*
    inline Vector4##suffix operator/(const Vector4##suffix & l, float f) \
  { \
    return Vector4##suffix((l.x / static_cast<type>(f)), (l.y / static_cast<type>(f)), (l.z / static_cast<type>(f)), (l.w / static_cast<type>(f))); \
    } \
    \
    inline Vector4##suffix operator+(const Vector4##suffix & l, const Vector4##suffix & r) \
  { \
    return Vector4##suffix((l.x + r.x), (l.y + r.y), (l.z + r.z), (l.w + r.w)); \
    } \
    \
    inline Vector4##suffix operator-(const Vector4##suffix & l, const Vector4##suffix & r) \
  { \
    return Vector4##suffix((l.x - r.x), (l.y - r.y), (l.z - r.z), (l.w - r.w)); \
    } \
    inline QString Vector4##suffix##ToString (const Vector4##suffix & v) { return QString("##Vector4##suffix##(x:%1;y:%2;z:%3;w:%4)").arg(v.x).arg(v.y).arg(v.z).arg(v.w);} \
    inline void PrintVector4##suffix (const Vector4##suffix & v) { LOG_TRACE(Vector4##suffix##ToString(v));}
};

*/

//=========================================================================================================================================
// Vector4 template
//=========================================================================================================================================


template< typename type >
class Vector3
{
public:
  type d[3];
  inline type&  x(){return eX;}
  inline type&  y(){return eY;}
  inline type&  z(){return eZ;}
  Q_DECL_CONSTEXPR inline type cx() const {return eX;}
  Q_DECL_CONSTEXPR inline type cy() const {return eY;}
  Q_DECL_CONSTEXPR inline type cz() const {return eZ;}
  Q_DECL_CONSTEXPR Vector3():Vector3(type(0)) {}
  Q_DECL_CONSTEXPR Vector3(type _x, type _y, type _z):d{_x, _y, _z}{}
  Q_DECL_CONSTEXPR Vector3(const Vector3& v):d{v.d[0], v.d[1],v.d[2]}{}
  Q_DECL_CONSTEXPR explicit Vector3(type* a):d{a[0], a[1], a[2]}{}
  Q_DECL_CONSTEXPR explicit Vector3(type v):d{v, v, v}{}
  Q_DECL_CONSTEXPR void set(type _x, type _y, type _z) { eX = _x; eY = _y; eZ = _z;}
  Q_DECL_CONSTEXPR void set(type val) { eX = val; eY = val; eZ = val;}
  Q_DECL_CONSTEXPR inline bool isZero() const {return (eX == 0) && (eY == 0) && (eZ == 0);}
  Q_DECL_CONSTEXPR inline bool isOne() const {return (eX == 1) && (eY == 1) && (eZ == 1);}


  void Print(bool endl = true) const
  {
    UNUSED(endl)
    LOG_TRACE(toString());
  }

  inline QString toString() const
  {
    return QString("%1; %2; %3")
        .arg(double(d[0]))
        .arg(double(d[1]))
        .arg(double(d[2]));
  }

#ifdef USE_EMBEDED_GLSETTERS
  inline void setAsGlTranslationF() const {glTranslatef(static_cast<GLfloat>(x), static_cast<GLfloat>(y), static_cast<GLfloat>(z));}
  inline void setAsGlTranslationD() const {glTranslated(static_cast<GLdouble>(x), static_cast<GLdouble>(y), static_cast<GLdouble>(z));}

  inline void setAsGlColorF() const {glColor3f(static_cast<GLfloat>(x), static_cast<GLfloat>(y), static_cast<GLfloat>(z));}
  inline void setAsGlColorD() const {glColor3d(static_cast<GLdouble>(x), static_cast<GLdouble>(y), static_cast<GLdouble>(z));}
  inline void setAsGlClearColor() const {glClearColor(static_cast<GLfloat>(x), static_cast<GLfloat>(y), static_cast<GLfloat>(z), 1.0f);}
  inline void setAsGlVertexF() const {glVertex3f(static_cast<GLfloat>(x), static_cast<GLfloat>(y), static_cast<GLfloat>(z));}
  inline void setAsGlVertexD() const {glVertex3d(static_cast<GLdouble>(x), static_cast<GLdouble>(y), static_cast<GLdouble>(z));}

  inline void setAsGlScaleF() const {glScalef(static_cast<GLfloat>(x), static_cast<GLfloat>(y), static_cast<GLfloat>(z));}
  inline void setAsGlScaleD() const {glScaled(static_cast<GLdouble>(x), static_cast<GLdouble>(y), static_cast<GLdouble>(z));}

  inline void setAsGlRotationF() const {glRotatef(static_cast<GLfloat>(x), 1, 0, 0); glRotatef(static_cast<GLfloat>(y), 0, 1, 0); glRotatef(static_cast<GLfloat>(z), 0, 0, 1);}
  inline void setAsGlRotationD() const {glRotated(static_cast<GLdouble>(x), 1, 0, 0); glRotatef(static_cast<GLdouble>(y), 0, 1, 0); glRotatef(static_cast<GLdouble>(z), 0, 0, 1);}
#endif
  Q_DECL_CONSTEXPR operator const type*() const { return d; }
  operator QVector3D() const {return QVector3D(static_cast<float>(x()),static_cast<float>(y()),static_cast<float>(z()));}
  type Length() const
  {
    return type(sqrtf(static_cast<float>((eX * eX) + (eY * eY) + (eZ * eZ))));
  }
  Vector3<type>& Normalize()
  {
    const type l = Length();
    eX /= l; eY /= l; eZ /= l;
    return *this;
  }
  Q_DECL_CONSTEXPR inline Vector3<type>& operator+=(const Vector3<type>& r) { eX += r.x(); eY += r.y(); eZ += r.z(); return *this; }
  Q_DECL_CONSTEXPR inline Vector3<type>& operator-=(const Vector3<type>& r) { eX -= r.x(); eY -= r.y(); eZ -= r.z(); return *this; }
  Q_DECL_CONSTEXPR inline Vector3<type>& operator+=(type v) { eX += v; eY += v; eZ += v; return *this; }
  Q_DECL_CONSTEXPR inline Vector3<type>& operator-=(type v) { eX -= v; eY -= v; eZ -= v; return *this; }
  Q_DECL_CONSTEXPR inline Vector3<type> operator+(const Vector3<type>& r) const { return Vector3<type>(eX + r.x(), eY + r.y(), eZ + r.z());}
  Q_DECL_CONSTEXPR inline Vector3<type> operator-(const Vector3<type>& r) const { return Vector3<type>(eX - r.x(), eY - r.y(), eZ - r.z());}
  Q_DECL_CONSTEXPR inline size_t mem_size() const {return 3 * sizeof(type);}

  Q_DECL_CONSTEXPR Vector3<type> Cross(const Vector3<type>& v) const
  {
    Q_DECL_CONSTEXPR  type _x = cy() * v.cz() - cz() * v.cy();
    Q_DECL_CONSTEXPR  type _y = cz() * v.cx() - cx() * v.cz();
    Q_DECL_CONSTEXPR  type _z = cx() * v.cy() - cy() * v.cx();

    return Vector3<type>(_x, _y, _z);
  }
};

// cannot be implemented as functions because implimentation of opengl functions depends of calling context
#define SetVector3AsGlTranslationF(v) glTranslatef(GLfloat(v.cx()), GLfloat(v.cy()), GLfloat(v.cz()))
#define SetVector3AsGlTranslationD(v) glTranslated(GLfloat(v.cx()), GLfloat(v.cy()), GLfloat(v.cz()))

#define SetVector3AsGlColorF(v) glColor3fv(v.d)
#define SetVector3AsGlColorD(v) glColor3dv(v.d)

#define SetVector3AsGlClearColor(v) glClearColor(GLfloat(v.cx()), GLfloat(v.cy()), GLfloat(v.cz()), 1.0f)

#define SetVector3AsGlVertexF(v) glVertex3fv(v.d)
#define SetVector3AsGlVertexD(v) glVertex3dv(v.d)

#define SetVector2AsGlVertex2F(v) glVertex2fv(v.d)
#define SetVector2AsGlVertex2D(v) glVertex2dv(v.d)

#define SetVector3AsGlVertex2F(v) SetVector2AsGlVertex2F(v)
#define SetVector3AsGlVertex2D(v) SetVector2AsGlVertex2D(v)

#define SetVector2AsGlTexCoord2F(v) glTexCoord2fv(v.d)
#define SetVector2AsGlTexCoord2D(v) glTexCoord2dv(v.d)

#define SetVector3AsGlTexCoord2F(v) SetVector2AsGlTexCoord2F(v)
#define SetVector3AsGlTexCoord2D(v) SetVector2AsGlTexCoord2D(v)

#define SetVector3AsGlScaleF(v) glScalef(GLfloat(v.cx()), GLfloat(v.cy()), GLfloat(v.cz()))
#define SetVector3AsGlScaleD(v) glScaled(GLfloat(v.cx()), GLfloat(v.cy()), GLfloat(v.cz()))

#define SetVector3AsGlRotationF(v) \
  glRotatef(GLfloat(v.cx()),  1.f, 0.f, 0.f); \
  glRotatef(GLfloat(v.cy()),  0.f, 1.f, 0.f); \
  glRotatef(GLfloat(v.cz()),  0.f, 0.f, 1.f)

#define SetVector3AsGlRotationD(v) \
  glRotated(GLdouble(v.cx()), 1.0, 0.0, 0.0); \
  glRotated(GLdouble(v.cy()), 0.0, 1.0, 0.0); \
  glRotated(GLdouble(v.cz()), 0.0, 0.0, 1.0)






//=========================================================================================================================================
// Vector2XXX types generator
//=========================================================================================================================================
template< typename type >
class Vector2
{
public:
  type d[2];
  inline type&  x(){return eX;}
  inline type&  y(){return eY;}
  Q_DECL_CONSTEXPR inline type cx() const {return eX;}
  Q_DECL_CONSTEXPR inline type cy() const {return eY;}
  Q_DECL_CONSTEXPR Vector2():Vector2(type(0)) {}
  Q_DECL_CONSTEXPR Vector2(type _x, type _y):d{_x, _y}{}
  Q_DECL_CONSTEXPR Vector2(const Vector2& v):d{v.d[0], d[1]}{}
  Q_DECL_CONSTEXPR explicit Vector2(type* a):d{a[0], a[1]}{}
  Q_DECL_CONSTEXPR explicit Vector2(type v):d{v, v}{}
  Q_DECL_CONSTEXPR void set(type _x, type _y) { eX = _x; eY = _y;}
  Q_DECL_CONSTEXPR void set(type val) { eX = val; eY = val;}
  Q_DECL_CONSTEXPR inline bool isZero() const {return (eX == 0) && (eY == 0);}
  Q_DECL_CONSTEXPR inline bool isOne() const {return (eX == 1) && (eY == 1);}


  void Print(bool endl = true) const
  {
    UNUSED(endl)
    LOG_TRACE(toString());
  }

  inline QString toString() const
  {
    return QString("%1; %2")
        .arg(static_cast<double>(x()))
        .arg(static_cast<double>(y()));
  }

  Q_DECL_CONSTEXPR operator const type*() const
  {
    return d;
  }

  operator QVector2D() const
  {
    return QVector2D(static_cast<float>(x()),static_cast<float>(y()));
  }

  Vector2<type>& Normalize()
  {
    const type Length = static_cast<type>(sqrtf(static_cast<float>((eX * eX) + (eY * eY))));
    eX /= Length; eY /= Length;
    return *this;
  }

  Q_DECL_CONSTEXPR inline Vector2<type>& operator+=(const Vector2<type>& r) { eX += r.x(); eY += r.y(); return *this; }
  Q_DECL_CONSTEXPR inline Vector2<type>& operator-=(const Vector2<type>& r) { eX -= r.x(); eY -= r.y(); return *this; }
  Q_DECL_CONSTEXPR inline Vector2<type>& operator+=(type v) { eX += v(); eY += v(); return *this; }
  Q_DECL_CONSTEXPR inline Vector2<type>& operator-=(type v) { eX -= v(); eY -= v(); return *this; }
  Q_DECL_CONSTEXPR inline Vector2<type> operator+(const Vector2<type>& r) const { return Vector2<type>(eX + r.x(), eY + r.y());}
  Q_DECL_CONSTEXPR inline Vector2<type> operator-(const Vector2<type>& r) const { return Vector2<type>(eX - r.x(), eY - r.y());}
  operator QSize() {return QSize(static_cast<int>(x()), static_cast<int>(y()));}

  Q_DECL_CONSTEXPR inline size_t mem_size() const {return 2 * sizeof(type);}
};


template< typename type >
class Size2
{
public:
  type d[2];
  inline type&  w(){return eX;}
  inline type&  h(){return eY;}
  Q_DECL_CONSTEXPR inline type cw() const {return eX;}
  Q_DECL_CONSTEXPR inline type ch() const {return eY;}
  Q_DECL_CONSTEXPR Size2():Size2(type(0)) {}
  Q_DECL_CONSTEXPR Size2(type _x, type _y):d{_x, _y}{}
  Q_DECL_CONSTEXPR Size2(const Size2& v):d{v.d[0], v.d[1]}{}
  Q_DECL_CONSTEXPR explicit Size2(type* a):d{a[0], a[1]}{}
  Q_DECL_CONSTEXPR explicit Size2(type v):d{v, v}{}
  Q_DECL_CONSTEXPR void set(type _x, type _y) { eX = _x; eY = _y;}
  Q_DECL_CONSTEXPR void set(type val) { eX = val; eY = val;}
  Q_DECL_CONSTEXPR inline bool isZero() const {return (eX == 0) && (eY == 0);}
  Q_DECL_CONSTEXPR inline bool isOne() const {return (eX == 1) && (eY == 1);}


  void Print(bool endl = true) const
  {
    UNUSED(endl)
    LOG_TRACE(toString());
  }

  inline QString toString() const
  {
    return QString("%1; %2")
        .arg(static_cast<double>(w()))
        .arg(static_cast<double>(h()));
  }

  Q_DECL_CONSTEXPR operator const type*() const
  {
    return d;
  }

  operator QVector2D() const
  {
    return QVector2D(float(w()), float(h()));
  }

  Size2<type>& Normalize()
  {
    const type Length = static_cast<type>(sqrtf(static_cast<float>((eX * eX) + (eY * eY))));
    eX /= Length; eY /= Length;
    return *this;
  }

  Q_DECL_CONSTEXPR inline Size2<type>& operator+=(const Size2<type>& r) { eX += r.w(); eY += r.h(); return *this; }
  Q_DECL_CONSTEXPR inline Size2<type>& operator-=(const Size2<type>& r) { eX -= r.w(); eY -= r.h(); return *this; }
  Q_DECL_CONSTEXPR inline Size2<type>& operator+=(type v) { eX += v; eY += v; return *this; }
  Q_DECL_CONSTEXPR inline Size2<type>& operator-=(type v) { eX -= v; eY -= v; return *this; }
  Q_DECL_CONSTEXPR inline Size2<type> operator+(const Size2<type>& r) const { return Size2<type>(eX + r.w(), eY + r.h());}
  Q_DECL_CONSTEXPR inline Size2<type> operator-(const Size2<type>& r) const { return Size2<type>(eX - r.w(), eY - r.h());}
  operator QSize() const { return QSize(int(eX), int(eY));}
  Q_DECL_CONSTEXPR inline float ratio() const
  {
    return static_cast<float>(w()) / static_cast<float>(h());
  }
  Q_DECL_CONSTEXPR inline size_t mem_size() const {return 2 * sizeof(type);}
};
/*
#define Vector2Type(type, suffix) \
  struct Vector2##suffix \
{ \
  type x = type(0); \
  type y = type(0); \
  \
  Vector2##suffix() {} \
  Vector2##suffix(type _x, type _y) { x = _x; y = _y;} \
  explicit Vector2##suffix(type* a) { x = a[0]; y = a[1];} \
  explicit Vector2##suffix(type v) { x = v; y = v;} \
  void set(type _x, type _y) { x = _x; y = _y;} \
  void set(type val) { x = val; y = val;} \
  inline bool isZero() const {return (x == 0) && (y == 0);} \
  inline bool isOne() const {return (x == 1) && (y == 1);} \
  void Print(bool endl = true) const { UNUSED(endl) LOG_TRACE(QString("%1.%2").arg(static_cast<double>(x)).arg(static_cast<double>(y))); } \
  operator const type*() const { return &(x); } \
  \
  Vector2##suffix& Normalize() \
{ \
  const type Length = static_cast<type>(sqrtf(static_cast<float>(x * x + y * y))); \
  x /= Length; y /= Length; \
  return *this; \
  } \
  \
  Vector2##suffix& operator+=(const Vector2##suffix& r) { x += r.x; y += r.y; return *this; } \
  Vector2##suffix& operator-=(const Vector2##suffix& r) { x -= r.x; y -= r.y; return *this; } \
  Vector2##suffix& operator+=(type v) { x += v; y += v; return *this; } \
  Vector2##suffix& operator-=(type v) { x -= v; y -= v; return *this; } \
  inline size_t mem_size() const {return 2*sizeof(type);} \
  }; \
  \
  inline Vector2##suffix operator/(const Vector2##suffix & l, float f) \
{ \
  return Vector2##suffix((l.x / static_cast<type>(f)), (l.y / static_cast<type>(f))); \
  } \
  \
  inline Vector2##suffix operator+(const Vector2##suffix & l, const Vector2##suffix & r) \
{ \
  return Vector2##suffix((l.x + r.x), (l.y + r.y)); \
  } \
  \
  inline Vector2##suffix operator-(const Vector2##suffix & l, const Vector2##suffix & r) \
{ \
  return Vector2##suffix((l.x - r.x), (l.y - r.y)); \
  } \
  inline QString Vector2##suffix##ToString (const Vector2##suffix & v) { return QString("##Vector2##suffix##(x:%1;y:%2)").arg(v.x).arg(v.y);} \
  inline void PrintVector2##suffix (const Vector2##suffix & v) { LOG_TRACE(Vector2##suffix##ToString(v));}

*/

//=========================================================================================================================================
// SizeXXX types generator
//=========================================================================================================================================
/*
#define Size2Type(type, suffix) \
  struct Size2##suffix \
{ \
  type w = type(0); \
  type h = type(0); \
  \
  Size2##suffix() {} \
  Size2##suffix(type _w, type _h) { w = _w; h = _h;} \
  explicit Size2##suffix(type* a) { w = a[0]; h = a[1];} \
  explicit Size2##suffix(type v) { w = v; h = v;} \
  explicit Size2##suffix(const QSize& s) {w = static_cast<type>(s.width()), h = static_cast<type>(s.height()); } \
  void set(type _w, type _h) { w = _w; h = _h;} \
  void set(type val) { w = val; h = val;} \
  operator QSize() {return QSize(static_cast<int>(w), static_cast<int>(h));} \
  \
  void Print(bool endl = true) const \
{ \
  UNUSED(endl) \
  LOG_TRACE(QString("%1.%2").arg(static_cast<double>(w)).arg(static_cast<double>(h))); \
  } \
  \
  operator const type*() const { return &(w); } \
  operator const QSize() const { return QSize(static_cast<int>(w), static_cast<int>(h)); } \
  \
  Size2##suffix& operator+=(const Size2##suffix& r) \
{ \
  w += r.w; h += r.h; \
  return *this; \
  } \
  \
  Size2##suffix& operator-=(const Size2##suffix& r) \
{ \
  w -= r.w; h -= r.h; \
  return *this; \
  } \
  }; \
  \
  inline Size2##suffix operator/(const Size2##suffix & l, float f) \
{ \
  return Size2##suffix((l.w / static_cast<type>(f)), (l.h / static_cast<type>(f))); \
  } \
  \
  inline Size2##suffix operator+(const Size2##suffix & l, const Size2##suffix & r) \
{ \
  return Size2##suffix((l.w + r.w), (l.h + r.h)); \
  } \
  \
  inline Size2##suffix operator-(const Size2##suffix & l, const Size2##suffix & r) \
{ \
  return Size2##suffix((l.w - r.w), (l.h - r.h)); \
  } \
  inline QString Size2##suffix##ToString (const Size2##suffix & v) { return QString("##Size2##suffix##(x:%1;y:%2)").arg(v.w).arg(v.h);} \
  inline void PrintSize2##suffix (const Size2##suffix & v) { LOG_TRACE(Size2##suffix##ToString(v));}
*/

#undef eX
#undef eY
#undef eZ
#undef eW

// class injections
#define Vector4GetSet(type, vectorSuffix, varName) \
  protected: \
  Vector4##vectorSuffix m_##varName; \
  public: \
  inline type x() const {return m_##varName.d[0]; } \
  inline type y() const {return m_##varName.d[1]; } \
  inline type z() const {return m_##varName.d[2]; } \
  inline type w() const {return m_##varName.d[3]; } \
  inline type & xRef() {return m_##varName.x(); } \
  inline type & yRef() {return m_##varName.y(); } \
  inline type & zRef() {return m_##varName.z(); } \
  inline type & wRef() {return m_##varName.w(); } \
  inline Vector4##vectorSuffix varName() const {return m_##varName; } \
  inline Vector4##vectorSuffix & varName##Ref() {return m_##varName; } \
  inline void Set##X(type val) { m_##varName.x() = val;} \
  inline void Set##Y(type val) { m_##varName.y() = val;} \
  inline void Set##Z(type val) { m_##varName.z() = val;} \
  inline void Set##W(type val) { m_##varName.w() = val;}


#define Vector3GetSet(type, vectorSuffix, varName) \
  protected: \
  Vector3##vectorSuffix m_##varName; \
  public: \
  inline type x() const {return m_##varName.d[0]; } \
  inline type y() const {return m_##varName.d[1]; } \
  inline type z() const {return m_##varName.d[2]; } \
  inline type& xRef() {return m_##varName.x(); } \
  inline type& yRef() {return m_##varName.y(); } \
  inline type& zRef() {return m_##varName.z(); } \
  inline Vector3##vectorSuffix varName() const {return m_##varName; } \
  inline void Set##X(type val) { m_##varName.x() = val;} \
  inline void Set##Y(type val) { m_##varName.y() = val;} \
  inline void Set##Z(type val) { m_##varName.z() = val;}

#define Vector3GetSetEx(type, vectorSuffix, varName, prefix) \
  protected: \
  Vector3##vectorSuffix m_##varName; \
  bool m_##varName##Changed; \
  public: \
  inline type prefix##x() const {return m_##varName.d[0]; } \
  inline type prefix##y() const {return m_##varName.d[1]; } \
  inline type prefix##z() const {return m_##varName.d[2]; } \
  inline type& prefix##xRef() {return m_##varName.x(); } \
  inline type& prefix##yRef() {return m_##varName.y(); } \
  inline type& prefix##zRef() {return m_##varName.z(); } \
  inline Vector3##vectorSuffix prefix##Vector() const {return m_##varName; } \
  inline Vector3##vectorSuffix & prefix##VectorRef() {return m_##varName; } \
  inline void Set##prefix##X(type val) { m_##varName.x() = val; m_##varName##Changed = true;} \
  inline void Set##prefix##Y(type val) { m_##varName.y() = val; m_##varName##Changed = true;} \
  inline void Set##prefix##Z(type val) { m_##varName.z() = val; m_##varName##Changed = true;} \
  inline void prefix (type x, type y, type z) {m_##varName.x() = x; m_##varName.y() = y; m_##varName.z() = z; m_##varName##Changed = true;} \
  inline void prefix (type val) {m_##varName.x() = val; m_##varName.y() = val; m_##varName.z() = val; m_##varName##Changed = true;}

#define Vector2GetSet(type, vectorSuffix, varName) \
  protected: \
  Vector2##vectorSuffix m_##varName; \
  bool m_##varName##Changed; \
  public: \
  inline type x() const {return m_##varName.d[0]; } \
  inline type y() const {return m_##varName.d[1]; } \
  inline type & xRef() {return m_##varName.x(); } \
  inline type & yRef() {return m_##varName.y(); } \
  inline Vector2##vectorSuffix varName() const {return m_##varName;} \
  inline void Set##X(type val) { m_##varName.x() = val; m_##varName##Changed = true;} \
  inline void Set##Y(type val) { m_##varName.y() = val; m_##varName##Changed = true;} \
  inline void move(type x, type y) {m_##varName.x() = x; m_##varName.y() = y; m_##varName##Changed = true;}

#define Size2GetSet(type, objSuffix, varName) \
  protected: \
  Size2##objSuffix m_##varName; \
  bool m_##varName##Changed; \
  public: \
  inline type w() const {return m_##varName.d[0]; } \
  inline type h() const {return m_##varName.d[1]; } \
  inline type & wRef() {return m_##varName.w(); } \
  inline type & hRef() {return m_##varName.h(); } \
  inline QSize size() const { return m_##varName; } \
  inline void Set##W(type val) { m_##varName.w() = val; m_##varName##Changed = true;} \
  inline void Set##H(type val) { m_##varName.h() = val; m_##varName##Changed = true;}

template< typename type >
Q_DECL_CONSTEXPR Vector4<type> extendV3_V4(const Vector3<type>&v, type w){return Vector4<type>(v.cx(), v.cy(), v.cz(), w);}

#define V4ARGS(v) v.cx(), v.cy(), v.cz(), v.cw()
#define V3ARGS(v) v.cx(), v.cy(), v.cz()
#define V2ARGS(v) v.cx(), v.cy()
#define QColor2Args4I(c) c.red(), c.green(), c.blue(), c.alpha()
#define QColor2Args4F(c) c.redF(), c.greenF(), c.blueF(), c.alphaF()
#define QColor2Args3I(c) c.red(), c.green(), c.blue()
#define QColor2Args3F(c) c.redF(), c.greenF(), c.blueF()


typedef Size2<double> Size2d;
typedef Size2<float> Size2f;
typedef Size2<uint> Size2ui;
typedef Size2<int> Size2i;
typedef Size2<quint64> Size2ui64;
typedef Size2<qint64> Size2i64;

typedef Vector2<double> Vector2d;
typedef Vector2<float> Vector2f;
typedef Vector2<uint> Vector2ui;
typedef Vector2<int> Vector2i;
typedef Vector2<quint64> Vector2ui64;
typedef Vector2<qint64> Vector2i64;

typedef Vector3<double> Vector3d;
typedef Vector3<float> Vector3f;
typedef Vector3<uint> Vector3ui;
typedef Vector3<int> Vector3i;
typedef Vector3<quint64> Vector3ui64;
typedef Vector3<qint64> Vector3i64;

typedef Vector4<double> Vector4d;
typedef Vector4<float> Vector4f;
typedef Vector4<uint> Vector4ui;
typedef Vector4<int> Vector4i;
typedef Vector4<quint64> Vector4ui64;
typedef Vector4<qint64> Vector4i64;

template< typename type >
static void optimizeAngle(Vector2<type>& v)
{
  LOG_TRACE("before optimization(%f; %f)", v.x(), v.y());
  type round = (v.x() > 0)?360.0:-360.0;
  for(; qAbs<type>(v.x()) >= 360.0f; v.x() -= round);
  round = (v.y() > 0)?360.0f:-360.0f;
  for(; qAbs<type>(v.y()) >= 360.0f;  v.y()-=round);
  LOG_TRACE("after optimization(%f; %f)", v.x(), v.y());
}

typedef Vector3f Point3f;
typedef Vector4f Color4;
typedef Vector3f Color3;

inline Vector3f randomVector3f(quint32 seed, float min, float max)
{
  QRandomGenerator rnd(seed);
  double x = Tools::fitToLimits<double>(rnd.generateDouble(), double(min), double(max));
  double y = Tools::fitToLimits<double>(rnd.generateDouble(), double(min), double(max));
  double z = Tools::fitToLimits<double>(rnd.generateDouble(), double(min), double(max));
  return Vector3f(float(x), float(y), float(z));
}

inline Vector4f randomVector4f(quint32 seed, float min, float max)
{
  QRandomGenerator rnd(seed);
  double x = Tools::fitToLimits<double>(rnd.generateDouble(), double(min), double(max));
  double y = Tools::fitToLimits<double>(rnd.generateDouble(), double(min), double(max));
  double z = Tools::fitToLimits<double>(rnd.generateDouble(), double(min), double(max));
  double w = Tools::fitToLimits<double>(rnd.generateDouble(), double(min), double(max));
  return Vector4f(float(x), float(y), float(z), float(w));
}

inline Color3 randomColor3(quint32 seed)
{
  return randomVector3f(seed, 0.0f, 1.0f);
}

inline Color4 randomColor4(quint32 seed)
{
  return randomVector4f(seed, 0.0f, 1.0f);
}

#define TF(byte) (float(byte) / 255.f)
Q_DECL_CONSTEXPR inline Color3 LighterColor3(const Color3& c, float amount)
{
  return Color3(Tools::boundedSumm(c.cx(), float(amount), 0.f, 1.f),
                Tools::boundedSumm(c.cy(), float(amount), 0.f, 1.f),
                Tools::boundedSumm(c.cz(), float(amount), 0.f, 1.f));
}
Q_DECL_CONSTEXPR inline Color4 LighterColor4(const Color4& c, float amount)
{
  return Color4(Tools::boundedSumm(c.cx(), float(amount), 0.f, 1.f),
                Tools::boundedSumm(c.cy(), float(amount), 0.f, 1.f),
                Tools::boundedSumm(c.cz(), float(amount), 0.f, 1.f),
                Tools::boundedSumm(c.cw(), float(amount), 0.f, 1.f));
}
Q_DECL_CONSTEXPR inline Color3 DarkerColor3(const Color3& c, float amount)
{
  return Color3(Tools::boundedSub(c.cx(), float(amount), 0.f, 1.f),
                Tools::boundedSub(c.cy(), float(amount), 0.f, 1.f),
                Tools::boundedSub(c.cz(), float(amount), 0.f, 1.f));
}
Q_DECL_CONSTEXPR inline Color4 DarkerColor4(const Color4& c, float amount)
{
  return Color4(Tools::boundedSub(c.cx(), float(amount), 0.f, 1.f),
                Tools::boundedSub(c.cy(), float(amount), 0.f, 1.f),
                Tools::boundedSub(c.cz(), float(amount), 0.f, 1.f),
                Tools::boundedSub(c.cw(), float(amount), 0.f, 1.f));
}

template< typename type >
Q_DECL_CONSTEXPR inline type ToRadian(type x)
{
  return type((x * M_PI) / 180.0);
}

template< typename type >
Q_DECL_CONSTEXPR inline type ToDegree(type x)
{
  return type((x * 180.0) / M_PI);
}

Vector3f QToDegrees(const Quaternion& q);

enum class VectorParamName: int
{
  X,Y,Z,W,ANY
};

MAYBE_UNUSED_ATTR static QString VectorParamNameToString(VectorParamName v)
{
  switch(v)
  {
    case VectorParamName::X:{return "X";}
    case VectorParamName::Y:{return "Y";}
    case VectorParamName::Z:{return "Z";}
    case VectorParamName::W:{return "W";}
    case VectorParamName::ANY:{return "ANY";}
    default:{return "???";}
  }
}

typedef std::function<void(const QString& name, VectorParamName pn, float val)> OnChangeObserver;

class IVectorChangeObserver
{
public:
  virtual void onVecotorChanged(const QString& name, VectorParamName pn)
  {
    UNUSED(name)
    UNUSED(pn)
  }
  virtual ~IVectorChangeObserver(){}
};


//=========================================================================================================================================
// printers
//=========================================================================================================================================
template< typename type >
void printVectorArray(type *v, size_t count)
{
  if (v == nullptr)
  {
    LOG_WARNING("null array");
    return;
  }
  for(size_t i = 0; i < count; ++i)
  {
    LOG_TRACE("varr.0x%llu[%d]: %s", intptr_t(v), i, SCHR(v[i].toString()));
  }
}
#endif // VECTORX_H
