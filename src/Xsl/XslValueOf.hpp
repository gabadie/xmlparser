#ifndef _H_XSL_VALUEOF
#define _H_XSL_VALUEOF

#include "XslInstruction.hpp"

namespace Xsl
{

    /**
     * Comportment of an Instruction corresponding to "value_of" in an xsl document.
     * value-of search for the textual value of the xml element corresponding to the attribute "select"
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
            Xml::Element const * xslElement, Xml::Log &) const override;

    };

}

#endif //_H_XSL_VALUEOF
