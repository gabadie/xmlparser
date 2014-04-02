#ifndef _H_XSL_VALUEOF
#define _H_XSL_VALUEOF

#include "XslInstruction.hpp"

namespace Xsl
{

    /**
     * TODO: doc
     */
    class ValueOf final: public Instruction
    {
    public:
        /**
         * Overloads Xsl::Instruction::operator ()
         */
        virtual
        std::vector<Xml::Node *>
        operator () (Xml::Element const * context, Xml::Document const & xslDoc,
            Xml::Element const * xslElement, Xml::Log & xslLog) const override;

    };

}

#endif //_H_XSL_VALUEOF
