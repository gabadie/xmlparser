#ifndef _H_XSL_FOREACH
#define _H_XSL_FOREACH

#include "XslInstruction.hpp"

namespace Xsl
{

    /**
     * TODO: doc
     */
    class ForEach: public Instruction
    {
    public:
        /**
         * Overloads Xsl::Instruction::operator ()
         */
        virtual
        std::vector<Xml::Node *>
        operator () (Xml::Element const * context, Xml::Document const & xslDoc,
            Xml::Element const * forEachElement) const override;

    };

}

#endif //_H_XSL_FOREACH