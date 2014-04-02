#ifndef _H_MEMORY_LEAKTRACKER
#define _H_MEMORY_LEAKTRACKER

#include <string>

/*
 * @brief Allocate a memory block of size <size>
 *
 * @param size Size of the block to allocate
 * @param file File in which the allocation call is done
 * @param line Line number of the allocation call
 * @param array True to allocate an array, false otherwise.
 *
 * @return A pointer to the newly allocated memory block
 */
void * memory_alloc(std::size_t size, std::string const & file,
    int line, bool array);

/*
 * @brief Free a memory block
 *
 * @param ptr Pointer to the memory block
 * @param array True if ptr points to an array, false otherwise.
 */
void memory_free(void * ptr, bool array);

/*
 * Add the current delete call to the stack of memory operations.
 *
 * @param file File in which the allocation call is done
 * @param line Line number of the allocation call
 */
void stack_delete(std::string const & file, int line);

#endif //_H_MEMORY_LEAKTRACKER
