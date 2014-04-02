#ifndef _H_XSL_TEMPLATE
#define _H_XSL_TEMPLATE

#include "XslInstruction.hpp"


namespace Xsl
{

    /**
     * Default behavior of the XSL transformation algorithm : goes through the nodes recursively,
     * adds basic XML nodes and applies templates when possible.
     *
     * @param context the XML context we're applying the transform to
     * @param xslDoc the XSL document that is used for the transformation
     * @param xslLog the XSL transform's log
     *
     * @return a list XML nodes generated by the transformation
     */
    std::vector<Xml::Node *>
    applyDefaultTemplate(Xml::Node const * context, Xml::Document const & xslDoc, Xml::Log & xslLog);

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
     * Finds the template in <xslDoc> that matches the most the given <context> element.
     *
     * @param context the element for which we're looking for a template
     * @param xslDoc the XSL document where we're looking for templates
     * @param xslLog the XSL transform's log
     *
     * @return the Elements produced by the transformation of <context> with the template found, or the Nodes
     * to be add to the new transformed document if no template matches.
     */
    std::vector<Xml::Node *>
    findAndApplyTemplate(Xml::Element const * context, Xml::Document const & xslDoc, Xml::Log & xslLog);

    /**
     * Applies a given XSL template in a given context (XML element) and returns a list of
     * the generate XML nodes.
     *
     * @param context the context where the template <xslTemplate> is applied
     * @param xslDoc the XSL document that is used for the transformation
     * @param xslTemplate the XSL template that will be applied to <context>
     * @param xslLog the XSL transform's log
     *
     * @return a list XML nodes generated by the application of the given template
     */
    std::vector<Xml::Node *>
    applyTemplate(Xml::Element const * context, Xml::Document const & xslDoc,
                  Xml::Element const * xslTemplate, Xml::Log & xslLog);

}

#endif //_H_XSL_TEMPLATE
