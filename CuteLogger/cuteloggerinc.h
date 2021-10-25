#ifndef CUTELOGGERINC
#define CUTELOGGERINC

#include "Logger.h"
#include "FileAppender.h"
#include "ConsoleAppender.h"
//#define ENABLE_FUCTION_CALLS

#ifdef ENABLE_FUCTION_CALLS
#define LOG_FUNCTION_CALL LOG_TRACE("Function call")

//for functions that cannot be called (for not implemented functions)
#define LOG_FUNCTION_INCORRECT_CALL LOG_ERROR("Incorrect function call")
#else // ifdef ENABLE_FUCTION_CALLS
#define LOG_FUNCTION_INCORRECT_CALL
#define LOG_FUNCTION_CALL
#endif // ifdef ENABLE_FUCTION_CALLS
#endif // CUTELOGGERINC

