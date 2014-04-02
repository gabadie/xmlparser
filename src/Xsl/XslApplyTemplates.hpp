#ifndef _H_XSL_APPLYTEMPLATES
#define _H_XSL_APPLYTEMPLATES

#include "XslInstruction.hpp"

namespace Xsl
{

    /**
     * TODO: doc
     */
    class ApplyTemplates: public Instruction
    {
    public:
        /**
         * Overloads Xsl::Instruction::operator ()
         */
        virtual
        std::vector<Xml::Node *>
        operator () (Xml::Element const * context, Xml::Document const & xslDoc,
            Xml::Element const * applyTemplatesElement) const override;

    };

}

#endif //_H_XSL_APPLYTEMPLATES
