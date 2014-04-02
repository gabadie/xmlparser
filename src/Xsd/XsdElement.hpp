#ifndef _H_XSD_ELEMENT
#define _H_XSD_ELEMENT

#include "XsdInstruction.hpp"


namespace Xsd
{

    class Element final: public Instruction
    {
    public:

        virtual
        bool
        validate(
            Xml::Element const * xmlElement,
            Xml::Document const * xsdDocument,
            Xml::Element const * xsdElement
        ) const;

        virtual
        std::string
        regex(
            Xml::Element const * xmlElement,
            Xml::Document const * xsdDocument,
            Xml::Element const * xsdElement
        ) const;

    };

}

#endif //_H_XSD_ELEMENT
