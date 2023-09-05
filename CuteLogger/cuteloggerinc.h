#ifndef CUTELOGGERINC
#define CUTELOGGERINC

#include "Logger.h"
#include "FileAppender.h"
#include "ConsoleAppender.h"
#include <QElapsedTimer>

#ifdef ENABLE_FUCTION_CALLS
#define LOG_FUNCTION_CALL LOG_TRACE("Function call")
#define LOG_FUNCTION_ENTRY LOG_TRACE("Function entry")
#define LOG_FUNCTION_EXIT LOG_TRACE("Function exit")
#define LOG_FUNCTION_ENTRY_EX QElapsedTimer __function_timer; LOG_TRACE("Function entry")
#define LOG_FUNCTION_EXIT_EX LOG_TRACE("Function exit. %lld", __function_timer.elapsed())
#define PrintValues(...) {QString __s; QTextStream ss(s); ss << "values: " << std::make_tuple(__VA_ARGS__); LOG_DEBUG(__s);}
// for functions that cannot be called (for not implemented functions)
#define LOG_FUNCTION_INCORRECT_CALL LOG_ERROR("Incorrect function call")
#else // ifdef ENABLE_FUCTION_CALLS
#define LOG_FUNCTION_INCORRECT_CALL
#define LOG_FUNCTION_CALL
#endif // ifdef ENABLE_FUCTION_CALLS
#endif // CUTELOGGERINC

