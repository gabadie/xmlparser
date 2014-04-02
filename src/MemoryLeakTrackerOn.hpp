#include "MemoryLeakTracker.hpp"

inline void * operator new(std::size_t size, char const * file, int line)
{
    return memory_alloc(size, file, line, false);
}

inline void * operator new[](std::size_t size, char const * file, int line)
{
    return memory_alloc(size, file, line, true);
}

inline void operator delete(void * ptr) throw()
{
    memory_free(ptr, false);
}

inline void operator delete[](void * ptr) throw()
{
    memory_free(ptr, true);
}

#ifndef new
#define new new(__FILE__, __LINE__)
#define delete stack_delete(__FILE__, __LINE__), delete
#endif
