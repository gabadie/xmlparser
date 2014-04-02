#ifndef _H_XSL_FOREACH
#define _H_XSL_FOREACH

#include "XslInstruction.hpp"

namespace Xsl
{
    /**
     * Comportment of an Instruction corresponding to "foreach" in an xsl document.
     * for-each loops loops through each node in the specified "select" node set and applies the 
     * contained xsl instructions .
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
            Xml::Element const * forEachElement) const override;

    };

}

#endif //_H_XSL_FOREACH
