#include <utility>

#include "XmlElement.hpp"
#include "XmlProcessingInstruction.hpp"

namespace Xml
{
    template <typename ...KeyValues>
    inline
    void
    Element::appendProcessingInstruction(std::string const & name, KeyValues && ...keyValues)
    {
        this->appendNode(new ProcessingInstruction(name, std::forward<KeyValues>(keyValues)...));
    }
}
