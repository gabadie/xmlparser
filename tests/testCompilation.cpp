

#include <mk_test.h>


void
test_hello_world()
{
    test_assert(1 != 0);
    test_passed("hello world");
}


int
main()
{
    test_hello_world();

    return 0;
}
