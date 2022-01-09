#ifndef BUILDCONSTANTS_H
#define BUILDCONSTANTS_H

#include <QStringList>
#include <QString>
#include <QAtomicInt>
#include "toolsmacro.h"

namespace Tools
{
#ifdef CFG_USERAGENT
CONST_LITERAL NETWORK_UDER_AGENT_NAME(CFG_USERAGENT);
#else
CONST_LITERAL NETWORK_UDER_AGENT_NAME("noname useragent");
#endif

#ifdef BUILD_TARGET
CONST_LITERAL TARGET_NAME(BUILD_TARGET);
#else
CONST_LITERAL TARGET_NAME("unnamed app");
#endif

#ifdef BUILD_QMAKE_FLAGS
CONST_LITERAL BUILDCFG_QMAKE_FLAGS(BUILD_QMAKE_FLAGS);
#else
CONST_LITERAL BUILDCFG_QMAKE_FLAGS("unknown");
#endif

CONST_LITERAL FEATURES_END("END");
MAYBE_UNUSED_ATTR const QStringList BUILDCFG_FEATURES
{
        #ifdef MEM_CONTROL_LOG
            LITERAL_TO_STRING(MEM_CONTROL_LOG),
        #endif

        #ifdef MEM_CONTROL_CSV
            LITERAL_TO_STRING(MEM_CONTROL_CSV),
        #endif

        #ifdef MUTEX_TRACE
            LITERAL_TO_STRING(MUTEX_TRACE),
        #endif

        #ifdef NETWORK_TRACE
            LITERAL_TO_STRING(NETWORK_TRACE),
        #endif

        #ifdef PERF_MON
            LITERAL_TO_STRING(PERF_MON),
        #endif

        #ifdef FEATURE_OBJ_COUNTER
            LITERAL_TO_STRING(FEATURE_OBJ_COUNTER),
        #endif

        #ifdef FEATURE_UNIT_TEST
            LITERAL_TO_STRING(FEATURE_UNIT_TEST),
        #endif

        #ifdef FEATURE_BOOST
            LITERAL_TO_STRING(FEATURE_BOOST),
        #endif

        #ifdef FEATURE_OPENGL
            LITERAL_TO_STRING(FEATURE_OPENGL),
        #endif

        #ifdef TOOLLIB_OPENCL
            LITERAL_TO_STRING(TOOLLIB_OPENCL),
        #endif

        #ifdef FEATURE_OPENGL
            LITERAL_TO_STRING(FEATURE_OPENGL),
        #endif

        #ifdef FEATURE_MULTITHREADS
            LITERAL_TO_STRING(FEATURE_MULTITHREADS),
        #endif

        #ifdef FEATURE_DATAFORMATS_INFO
            LITERAL_TO_STRING(FEATURE_DATAFORMATS_INFO),
        #endif

        FEATURES_END
}; // const QStringList BUILDCFG_FEATURES

class ConstantsHelper
{
public:
    static QString getFeaturesStr(const QString& separator = QString(" "));
    static QString getAtomicAbilitiesStr(const QString& separator = QString(" "));
    static void printBuldConstants();
private:
    DISABLE_INSTANTIATING(ConstantsHelper)
}; // class ConstantsHelper

} // namespace Tools
#endif // BUILDCONSTANTS_H
