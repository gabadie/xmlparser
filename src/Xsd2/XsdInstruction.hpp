#ifndef _H_XSD_INSTRUCTION
#define _H_XSD_INSTRUCTION

#include <vector>
#include "../Xml/Xml.hpp"


namespace Xsd
{

    class Instruction
    {
    public:

        virtual
        bool
        validate(
            Xml::Element const * xmlElement,
            Xml::Document const * xsdDocument,
            Xml::Element const * xsdElement
        ) const = 0;

        virtual
        std::string
        regex(
            Xml::Element const * xmlElement,
            Xml::Document const * xsdDocument,
            Xml::Element const * xsdElement
        ) const = 0;

    };

}

#endif