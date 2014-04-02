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
        std::vector <Xml::Node *>
        operator () (
            Xml::Element const * context,
            Xml::Document const & xslDoc,
            Xml::Element const * forEachElement,
            Xml::Log & xslLog
        ) const = 0;

    };

}

#endif
