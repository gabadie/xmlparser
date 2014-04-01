#ifndef _H_XSL_FOREACH
#define _H_XSL_FOREACH

#include "Xsl.hpp"

namespace Xsl
{
    class ForEach: public Instruction
    {
    public:
        virtual
        std::vector<Xml::Node *>
        operator () (Xml::Element const * context, const Xml::Document& xslDoc,  Xml::Element const * forEachElement) const;
    };
}

#endif //_H_XSL_FOREACH