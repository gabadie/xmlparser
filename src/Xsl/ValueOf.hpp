#ifndef _H_XSL_VALUEOF
#define _H_XSL_VALUEOF

#include "Xsl.hpp"

namespace Xsl
{
    class ValueOf : public Instruction
    {
    public:
        virtual
        std::vector<Xml::Node *>
        operator () (Xml::Element const * context,  const Xml::Document& xslDoc,  Xml::Element const * xslElement) const;
    };
}

#endif //_H_XSL_VALUEOF