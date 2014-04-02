#ifndef _H_XSL_FOREACH
#define _H_XSL_FOREACH

#include "XslInstruction.hpp"

namespace Xsl
{
    class ForEach: public Instruction
    {
    public:
        virtual
        std::vector<Xml::Node *>
        operator () (Xml::Element const * context, Xml::Document const & xslDoc,
            Xml::Element const * forEachElement) const override;
    };
}

#endif //_H_XSL_FOREACH
