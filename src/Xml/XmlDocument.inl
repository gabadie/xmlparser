#include <utility>

#include "XmlDocument.hpp"
#include "XmlProcessingInstruction.hpp"

namespace Xml
{
    template <typename ...KeyValues>
    inline
    void
    Document::appendProcessingInstruction(std::string const & name, KeyValues && ...keyValues)
    {
        this->appendNode(new ProcessingInstruction(name, std::forward<KeyValues>(keyValues)...));
    }
}
