/**
 * \file XmlProcessingInstruction.cpp
 * \brief XML processing instruction
 * \author tcantenot
 * \version 0.1
 * \date 18 mars 2014
 */

#include <iostream>

#include "XmlProcessingInstruction.hpp"
#include "../Utils.hpp"

#include "../MemoryLeakTrackerOn.hpp"

namespace Xml
{

    ProcessingInstruction::~ProcessingInstruction()
    {

    }

    ProcessingInstruction::ProcessingInstruction(ProcessingInstruction const & piTocopy) :
        DocumentNode(),
        mAttributes(piTocopy.mAttributes)
    {
    }

    ObjectLabel
    ProcessingInstruction::objectLabel() const
    {
        return ObjectLabel::ProcessingInstruction;
    }

    Node *
    ProcessingInstruction::clone() const
    {
        return new ProcessingInstruction(*this);
    }

    std::string const &
    ProcessingInstruction::attribute(std::string const & name) const
    {
        static std::string const notFound = "";

        auto it = mAttributes.find(name);

        return (it != std::end(mAttributes)) ? it->second : notFound;
    }

    void
    ProcessingInstruction::setAttribute(std::string const & name, std::string const & value)
    {
        if(name.size() > 0)
        {
            mAttributes[name] = value;
        }
    }

    void
    ProcessingInstruction::exportToStream(std::ostream & stream, std::size_t level,
        std::string const & indent) const
    {
        stream << Utils::repeat(indent, level) << "<?" << mName;

        for(auto const & a : mAttributes)
        {
            stream << " " << a.first << "=\"" << a.second << "\"";
        }

        stream << "?>";
    }
}

#include "../MemoryLeakTrackerOff.hpp"
