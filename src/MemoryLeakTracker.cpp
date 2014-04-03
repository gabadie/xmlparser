#include "MemoryLeakTracker.hpp"

#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <stack>

namespace
{
    class MemoryLeakTracker
    {
    public:
        MemoryLeakTracker() = default;
        ~MemoryLeakTracker();
    } mlt;

    struct MemBlockInfo
    {
        std::size_t size;
        std::string file;
        int line;
        bool array;

        MemBlockInfo(std::size_t s = 0u, std::string const & f = "",
            int l = 0, bool a = false):
            size(s), file(f), line(l), array(a)
        {

        }

        MemBlockInfo(MemBlockInfo &&) = default;
        MemBlockInfo & operator=(MemBlockInfo &&) = default;

        MemBlockInfo(MemBlockInfo const &) = delete;
        MemBlockInfo & operator=(MemBlockInfo const &) = delete;
    };

    using MemBlockInfoMap = std::map<void *, MemBlockInfo>;
    using MemBlockInfoStack = std::stack<MemBlockInfo>;

    MemBlockInfoMap memBlocks;
    MemBlockInfoStack deleteStack;
    std::size_t errors(0);
    std::stringstream log;

    void printInfo(std::string const & action, void * ptr,
        std::size_t size, std::string const & file, int line)
    {
        log << "[" << action << "] : " << std::setw(10) << ptr;
        log << " | " << std::setw(5) << std::setfill(' ');
        log << static_cast<int>(size) << " byte" << (size > 1 ? "s" : " ");
        log << " | " << file << " |" << line << "|";
        log << std::endl;
    }

    void reportLeaks()
    {
        std::size_t totalSize(0);

        for(auto & it : memBlocks)
        {
            auto ptr = it.first;
            auto const & block = it.second;

            totalSize += block.size;

            #ifdef APP_MEMORY_TRACE
            printInfo("Leak", ptr, block.size, block.file, block.line);
            #else
            std::cerr << "MEMORY LEAK OF " << ptr << std::endl << " AT " << block.file << ":" << block.line << std::endl;
            #endif

            //std::free(ptr);
        }


        auto nbBlocks = static_cast<int>(memBlocks.size());
        auto totalLoss = static_cast<int>(totalSize);

        auto pluralize = [](std::string const & w, std::size_t count,
            std::string const & singular) {
            return w + (count > 1 ? "s" : singular);
        };

        #ifndef APP_MEMORY_TRACE
        #define log std::cerr
        #endif

        log << std::endl;
        log << "=> ";
        log << nbBlocks << " leaked " << pluralize("block", nbBlocks, "");
        log << " : " << totalLoss << pluralize(" byte", totalLoss, " ") << " lost.";
        log << std::endl;

        #ifndef APP_MEMORY_TRACE
        #undef log
        #endif
    }

    MemoryLeakTracker::~MemoryLeakTracker()
    {
        bool leaks = !memBlocks.empty();

        if(!leaks)
        {
            log << std::endl;
            log << "=======================================" << std::endl;
            log << "            No leak detected !         " << std::endl;
            log << "=======================================" << std::endl;
            log << std::endl;
        }
        else
        {
            #ifndef APP_MEMORY_TRACE
            #define log std::cerr
            #endif

            log << std::endl;
            log << "=======================================" << std::endl;
            log << "     Some leaks have been detected     " << std::endl;
            log << "=======================================" << std::endl;
            log << std::endl;

            #ifndef APP_MEMORY_TRACE
            #undef log
            #endif

            reportLeaks();
        }

        if(errors > 0)
        {
            log << std::endl;
            log << "[WARNING] : " << errors << " error" << (errors > 1 ? "s" : "")
                << " have been detected." << std::endl;
        }

        #ifdef APP_MEMORY_TRACE
        std::cerr << "\n>>> MEMORY TRACE <<<" << std::endl;
        std::cerr << log.str();
        #else
        if(leaks)
        {
            //std::cerr << "\n>>> MEMORY TRACE <<<" << std::endl;
            //std::cerr << log.str();
        }
        #endif
    }
}


void * memory_alloc(std::size_t size, std::string const & file,
    int line, bool array)
{
    void * ptr = std::malloc(size);

    memBlocks.insert(std::make_pair(ptr, MemBlockInfo(size, file, line, array)));

    printInfo(" Allocation ", ptr, size, file, line);

    return ptr;
}

void memory_free(void * ptr, bool array)
{
    auto const it = memBlocks.find(ptr);

    if(it == std::end(memBlocks))
    {
        std::free(ptr);
        return;
    }

    auto const & block = it->second;
    if(block.array != array)
    {
        if(array)
        {
            log << std::endl;
            log << " [ERROR] : " << block.file << "|" <<  block.line
                << "| ~> Mismatched operations : delete[] after new" << std::endl;
            log << std::endl;

            ++errors;
        }
        else
        {
            log << std::endl;
            log << " [ERROR] : " << block.file << "|" <<  block.line
                << "| ~> Mismatched operations : delete after new[]" << std::endl;
            log << std::endl;

            ++errors;
        }
    }

    if(!deleteStack.empty())
    {
        printInfo("Deallocation", ptr, block.size, deleteStack.top().file, deleteStack.top().line);
        deleteStack.pop();
    }
    else
    {
        log << "Deallocation of " << ptr << " from outside MemoryLeakTracker reach" << std::endl;
    }

    memBlocks.erase(it);

    std::free(ptr);
}

void stack_delete(std::string const & file, int line)
{
    deleteStack.emplace(0, file, line, false);
}
