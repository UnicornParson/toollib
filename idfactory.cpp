#include "idfactory.h"

using namespace Tools;

IdFactory::IdFactory():
    nextId(1)
{

}

IdFactory::IdFactory(const IdFactory& other):nextId(other.nextId)
{

}

IdFactory& IdFactory::operator=(const IdFactory& other)
{
    if (this != &other)
    {
        nextId = other.nextId;
    }
    return *this;
}
quint64 IdFactory::newId()
{
    return nextId.fetchAndAddOrdered(1);
}

quint64 IdFactory::getLastId() const
{
    return (quint64)nextId;
}

void IdFactory::reset(quint64 firstId)
{
    nextId.storeRelease(firstId);
}
