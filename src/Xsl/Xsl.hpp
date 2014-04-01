#ifndef _H_XSL_XSL
#define _H_XSL_XSL

#include <vector>

#include "../Xml/XmlForward.hpp"

namespace Xsl
{
    /**
     * Defines a XML document
     */
    class Instruction
    {
    public:
        virtual std::vector <Xml::Node *>
        operator () (Xml::Element const * context, Xml::Document const & xslDoc,
            Xml::Element const * forEachElement) const = 0;
    };

    Xml::Document *
    xslTransform(Xml::Document const & xmlDoc, Xml::Document const & xslDoc);

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


    std::vector<Xml::Node *>
    findAndApplyTemplate(Xml::Element const * context, Xml::Document const & xslDoc);

    std::vector<Xml::Node *>
    applyTemplate(Xml::Element const * context, Xml::Document const & xslDoc, Xml::Element const * xslTemplate);

}
#endif //_H_XSL_XSL
