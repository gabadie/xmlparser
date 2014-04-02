#ifndef _H_XSL_FOREACH
#define _H_XSL_FOREACH

#include "XslInstruction.hpp"

namespace Xsl
{
    /**
     * TODO : Doc
     */
    class ForEach final: public Instruction
    {
    public:
        /**
         * Overloads Xsl::Instruction::operator ()
         */
        virtual
        std::vector<Xml::Node *>
        operator () (Xml::Element const * context, Xml::Document const & xslDoc,
            Xml::Element const * forEachElement, Xml::Log & xslLog) const override;

    };

}

#endif //_H_XSL_FOREACH
