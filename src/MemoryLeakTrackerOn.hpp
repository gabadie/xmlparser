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

//#include <cstddef>

//extern char const * __file__;
//extern std::size_t __line__;

//inline void * operator new(std::size_t size)
//{
    //return nullptr;
    ////return memory_alloc(size, __file__, __line__, false);
//}

//inline void * operator new[](std::size_t size)
//{
    //return memory_alloc(size, __file__, __line__, true);
//}

//inline void operator delete(void * ptr) throw()
//{
    //memory_free(ptr, false);
//}

//inline void operator delete[](void * ptr) throw()
//{
    //memory_free(ptr, true);
//}

//#ifndef new
////#define new new(__FILE__, __LINE__)
////#define delete stack_delete(__FILE__, __LINE__), delete
//#define new (__file__=__FILE__, __line__=__LINE__) && false ? nullptr : new
////#define delete stack_delete(__FILE__, __LINE__), delete

