#include "quaternion.h"
#include <QtMath>
#include "engineTypes.h"

Quaternion::Quaternion():
    m_x(0.0f),
    m_y(0.0f),
    m_z(0.0f),
    m_w(0.0f)
{

}

Quaternion::Quaternion(float _x, float _y, float _z, float _w)
{
    m_x = _x;
    m_y = _y;
    m_z = _z;
    m_w = _w;
}

void Quaternion::Normalize()
{
    float Length = sqrtf(m_x * m_x
                         + m_y * m_y
                         + m_z * m_z
                         + m_w * m_w);

    m_x /= Length;
    m_y /= Length;
    m_z /= Length;
    m_w /= Length;
}


Quaternion Quaternion::Conjugate()
{
    Quaternion ret(-m_x, -m_y, -m_z, m_w);
    return ret;
}

Quaternion operator*(const Quaternion& l, const Quaternion& r)
{
    const float w = (l.w() * r.w()) - (l.x() * r.x()) - (l.y() * r.y()) - (l.z() * r.z());
    const float x = (l.x() * r.w()) + (l.w() * r.x()) + (l.y() * r.z()) - (l.z() * r.y());
    const float y = (l.y() * r.w()) + (l.w() * r.y()) + (l.z() * r.x()) - (l.x() * r.z());
    const float z = (l.z() * r.w()) + (l.w() * r.z()) + (l.x() * r.y()) - (l.y() * r.x());

    Quaternion ret(x, y, z, w);

    return ret;
}

Quaternion operator*(const Quaternion& q, const Vector3f& v)
{
    const float w = - (q.x() * v.cx()) - (q.y() * v.cy()) - (q.z() * v.cz());
    const float x =   (q.w() * v.cx()) + (q.y() * v.cz()) - (q.z() * v.cy());
    const float y =   (q.w() * v.cy()) + (q.z() * v.cx()) - (q.x() * v.cz());
    const float z =   (q.w() * v.cz()) + (q.x() * v.cy()) - (q.y() * v.cx());

    Quaternion ret(x, y, z, w);

    return ret;
}

