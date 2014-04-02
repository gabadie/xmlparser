#ifndef _H_XSL_APPLYTEMPLATES
#define _H_XSL_APPLYTEMPLATES

#include "XslInstruction.hpp"

namespace Xsl
{

    /**
     * ApplyTemplates implements the apply-templates XSL instruction.
     * It looks for all elements that match its "select" attribute and applies
     * their templates (if any)
     */
    class ApplyTemplates final: public Instruction
    {
    public:
        /**
         * Overloads Xsl::Instruction::operator ()
         */
        virtual
        std::vector<Xml::Node *>
        operator () (Xml::Element const * context, Xml::Document const & xslDoc,
            Xml::Element const * applyTemplatesElement, Xml::Log & xslLog) const override;

    };

}

#endif //_H_XSL_APPLYTEMPLATES
