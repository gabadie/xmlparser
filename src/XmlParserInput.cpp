
#include "XmlParserInput.hpp"


namespace
{
    std::istream * flexInputStream = 0;
}

namespace Xml
{

    void
    flexSetInput(std::istream & inputStream)
    {
        flexInputStream = &inputStream;
    }

    int
    flexInput(char * buffer, int bufferMaxSize)
    {
        return flexInputStream->readsome(buffer, bufferMaxSize);
    }

}
