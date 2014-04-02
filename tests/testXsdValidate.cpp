#include "testhelper.hpp"

/*
 * To test the Xml parser, we need to access some protected/private field
 */
#define protected public
#define private public

#include "../src/Xsd/XsdValidate.hpp"

void
testXsdValidateIsDate()
{
    test_assert(Xsd::isDate("2014-01-01"));
    test_assert(Xsd::isDate("2014-12-31"));
    test_assert(!Xsd::isDate("01-01-2014"));
    test_assert(!Xsd::isDate("01-2014-01"));
}

int
main()
{
    testXsdValidateIsDate();

    return 0;
}
