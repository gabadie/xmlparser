#ifndef _H_TESTHELPER
#define _H_TESTHELPER

#include <mk_test.h>


/**
 * Converts plain XML code to (char const *)
 */
#define xml_code(code) \
    ((char const *) #code)


#endif
