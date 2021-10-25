#include "objectid.h"
#include "toolsmacro.h"
using namespace Tools;

quint64 ObjectId::s_nextId = 0;
XNonRecursiveMutex ObjectId::s_idMutex;
MAYBE_UNUSED_ATTR constexpr char alphabet[] = "0123456789abcdefghijklmnopqrstuvwxyz+-*=#@";

ObjectId::ObjectId()
{
    s_idMutex.lock();
    m_id = s_nextId;
    s_nextId++;
    s_idMutex.unlock();
    updateStr();
}

ObjectId::ObjectId(const ObjectId& other):
	m_id(other.m_id)
{
    updateStr();
}

void ObjectId::updateStr()
{
    const quint64 alphabetSize = strlen(alphabet);
    QString prefix;
    quint64 subindex = m_id;
    while (subindex >= alphabetSize)
    {
        prefix.insert(0, QChar(alphabet[subindex % alphabetSize]));
        subindex /= alphabetSize;
    }
    prefix.insert(0, QChar(alphabet[subindex % alphabetSize]));
    m_str = prefix;
}
