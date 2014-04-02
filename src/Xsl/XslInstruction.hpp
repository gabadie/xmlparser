#ifndef _H_XSL_INSTRUCTION
#define _H_XSL_INSTRUCTION

#include <vector>
#include "../Xml/Xml.hpp"


namespace Xsl
{

    /**
     * Defines a XSL instruction interface
     */
    class Instruction
    {
    public:

        /**
         * TODO: doc!
         */
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
