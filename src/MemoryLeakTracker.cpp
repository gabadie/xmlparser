#include "MemoryLeakTracker.hpp"

#include <iomanip>
#include <iostream>
#include <map>
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

    void printInfo(std::string const & action, void * ptr,
        std::size_t size, std::string const & file, int line)
    {
        std::cerr << "[" << action << "] : " << std::setw(10) << ptr;
        std::cerr << " | " << std::setw(5) << std::setfill(' ');
        std::cerr << static_cast<int>(size) << " byte" << (size > 1 ? "s" : " ");
        std::cerr << " | " << file << " |" << line << "|";
        std::cerr << std::endl;
    }

    void reportLeaks()
    {
        std::size_t totalSize(0);

        for(auto & it : memBlocks)
        {
            auto ptr = it.first;
            auto const & block = it.second;

            totalSize += block.size;

            printInfo("Leak", ptr, block.size, block.file, block.line);

            std::free(ptr);
        }

        auto nbBlocks = static_cast<int>(memBlocks.size());
        auto totalLoss = static_cast<int>(totalSize);

        auto pluralize = [](std::string const & w, std::size_t count,
            std::string const & singular) {
            return w + (count > 1 ? "s" : singular);
        };

        std::cerr << std::endl;
        std::cerr << "=> ";
        std::cerr << nbBlocks << " leaked " << pluralize("block", nbBlocks, "");
        std::cerr << " : " << totalLoss << pluralize(" byte", totalLoss, " ") << " lost.";
        std::cerr << std::endl;
    }

    MemoryLeakTracker::~MemoryLeakTracker()
    {
        if(memBlocks.empty())
        {
            std::cerr << std::endl;
            std::cerr << "=======================================" << std::endl;
            std::cerr << "            No leak detected !         " << std::endl;
            std::cerr << "=======================================" << std::endl;
            std::cerr << std::endl;
        }
        else
        {
            std::cerr << std::endl;
            std::cerr << "=======================================" << std::endl;
            std::cerr << "     Some leaks have been detected     " << std::endl;
            std::cerr << "=======================================" << std::endl;
            std::cerr << std::endl;

            reportLeaks();
        }

        if(errors > 0)
        {
            std::cerr << std::endl;
            std::cerr << "[WARNING] : " << errors << " error" << (errors > 1 ? "s" : "")
                << " have been detected." << std::endl;
        }

        std::cerr << std::endl;
        std::cerr << "--------------------------------------------------------------";
        std::cerr << std::endl << std::endl;
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
            std::cerr << std::endl;
            std::cerr << " [ERROR] : " << block.file << "|" <<  block.line
                << "| ~> Mismatched operations : delete[] after new" << std::endl;
            std::cerr << std::endl;

            ++errors;
        }
        else
        {
            std::cerr << std::endl;
            std::cerr << " [ERROR] : " << block.file << "|" <<  block.line
                << "| ~> Mismatched operations : delete after new[]" << std::endl;
            std::cerr << std::endl;

            ++errors;
        }
    }

    printInfo("Deallocation", ptr, block.size, deleteStack.top().file,
              deleteStack.top().line);

    memBlocks.erase(it);
    deleteStack.pop();

    std::free(ptr);
}

void stack_delete(std::string const & file, int line)
{
    deleteStack.emplace(0, file, line, false);
}
