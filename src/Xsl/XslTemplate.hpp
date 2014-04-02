#ifndef _H_XSL_TEMPLATE
#define _H_XSL_TEMPLATE

#include "XslInstruction.hpp"


namespace Xsl
{

    /**
     * Charged to return the new Nodes to add to the new document XML, applying a template on <context> if it has one, 
     * returning text elements if there are so.
     * @param <context> the Node to examine to apply a template or return the content.
     * @param <xslDoc> the document Xsl used for the transformation of document containing <context>
     */
    std::vector<Xml::Node *>
    applyDefaultTemplate(Xml::Node const * context, Xml::Document const & xslDoc);

    /**
     * Returns the appropriate template (if any) that should be applied
     * to the given element
     *
     * @param xslDoc The XSL document containing the templates
     * @param element The element that the returned template should be applied to
     *
     * @return the template that matches the given element. If no template matches, we return nullptr
     */
    Xml::Element const *
    getTemplate(Xml::Document const & xslDoc, Xml::Element const * element);

    /**
     * Search for a template to apply to <context> in <xslDoc> and apply it (if any). 
     * @param <context> Element on which we try to apply a matching template
     * @param <xslDoc> the document Xsl used for the transformation of the document containing <context>
     * @return the Elements produced by the transformation of <context> with the template found, or the Nodes
     * to be add to the new transformed document if no template matches.
     */
    std::vector<Xml::Node *>
    findAndApplyTemplate(Xml::Element const * context, Xml::Document const & xslDoc);

    /**
     * Apply the template <xslTemplate> to <context> 
     * @param <context> the Element Xml which has to be transformed
     * @param <xslDoc> the Xsl document we are using to transformed <context>
     * @param <xslTemplate> the template to apply to <context>
     */
    std::vector<Xml::Node *>
    applyTemplate(Xml::Element const * context, Xml::Document const & xslDoc, Xml::Element const * xslTemplate);

}

#endif //_H_XSL_TEMPLATE
