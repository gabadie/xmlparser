/**
 * \file AppDebug.hpp
 * \brief Headers defining all debug symbols.
 * \author Guillaume.A
 * \version 1.0
 * \date march 18th 2014
 */
#ifndef _APP_DEBUG
#define _APP_DEBUG

#ifdef APP_DEBUG
#include <stdio.h>

#endif


/*
 * @infos: generates a debug breakpoint when reached
 */
#ifdef APP_DEBUG
#define app_break_point() \
    __builtin_trap()

#else
#define app_break_point()

#endif //APP_DEBUG

/*
 * @infos: use app_crash instead
 */
#define app_crash_definitly() \
    do { \
        app_break_point(); \
        *((volatile int*) NULL) = 123; \
    } while (0)


/*
 * @infos: convert code to string
 */
#define app_to_str(code) #code


/*
 * @infos: crashes app execution
 */
#ifdef APP_DEBUG
#define app_crash() \
    { \
        printf("CRASH AT %s@%i\n", __FILE__, __LINE__); \
        app_crash_definitly(); \
    }

#else
#define app_crash()

#endif //APP_DEBUG


/*
 * @infos: crashes app execution with a message
 *
 * @params: like printf
 */
#ifdef APP_DEBUG
#define app_crash_msg(...) \
    { \
        printf("CRASH AT %s@%i in %s: ", __FILE__, __LINE__, __func__); \
        printf(__VA_ARGS__); \
        printf("\n"); \
        app_crash_definitly(); \
    }

#else
#define app_crash_msg(...)

#endif //APP_DEBUG

/*
 * @infos: definies a todo that crash when reached
 *
 * @params: like printf
 */
#ifdef APP_DEBUG
#define app_todo(...) \
    { \
        printf("TODO AT %s@%i in %s: ", __FILE__, __LINE__, __func__); \
        printf(__VA_ARGS__); \
        printf("\n"); \
        app_crash_definitly(); \
    }

#else
#define app_todo(...)

#endif //APP_DEBUG

/*
 * @infos: assert app execution
 *
 * @param <condition>: assertion's condition code to pass
 */
#ifdef APP_DEBUG
#define app_assert(condition) \
    if (!(condition)) \
    { \
        printf("ASSERTION ERROR AT %s@%i in %s: %s\n", __FILE__, __LINE__, __func__, app_to_str(condition)); \
        app_crash_definitly(); \
    }

#else
#define app_assert(condition)

#endif //APP_DEBUG

/*
 * @infos: not implemented feature crash
 */
#ifdef APP_DEBUG
#define app_not_implemented_yet \
    app_crash_msg("Not implemented yet");

#else
#define app_not_implemented_yet

#endif //APP_DEBUG

/*
 * @infos: unreachable crash
 */
#define app_unreachable() \
    app_crash_msg("This code is supposed to be unreachable");

#endif
