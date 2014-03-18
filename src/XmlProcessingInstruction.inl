#include "XmlProcessingInstruction.hpp"
#include <utility>

namespace Xml
{

template <typename ...KeyValues>
inline
ProcessingInstruction::ProcessingInstruction(std::string const & name, KeyValues && ...keyValues):
    DocumentNode(),
    mName(name),
    mAttributes()
{
    this->setAttribute(std::forward<KeyValues>(keyValues)...);
}

template <typename ...KeyValues>
inline
void
ProcessingInstruction::setAttribute(std::string const & name, std::string const & value,
    KeyValues && ...keyValues)
{
    this->setAttribute(name, value);
    this->setAttribute(std::forward<KeyValues>(keyValues)...);
}

}
