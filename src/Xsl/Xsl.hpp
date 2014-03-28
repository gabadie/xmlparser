#ifndef _H_XSL_XSL
#define _H_XSL_XSL

#include <vector>
#include <map>

#include "../Xml/XmlNode.hpp"
#include "../Xml/XmlText.hpp"
#include "../Xml/XmlDocument.hpp"


typedef std::vector< Xml::Node*> nodeVector;

namespace Xsl
{

    /**
     * Defines a XML document
     */
    class Instruction
    {
    public:
        virtual std::vector <Xml::Node*>  operator () (const Xml::Element* context, const Xml::Document& xslDoc,  const Xml::Element * forEachElement) const = 0;
    };


    class ValueOf : public Instruction
    {
    public:
        virtual std::vector <Xml::Node*>  operator () (const Xml::Element* context,  const Xml::Document& xslDoc,  const Xml::Element * xslElement) const;
    };

    class ForEach: public Instruction
    {
    public:
        virtual std::vector <Xml::Node*>  operator () (const Xml::Element* context, const Xml::Document& xslDoc,  const Xml::Element * forEachElement) const;
    };

    class ApplyTemplate: public Instruction
    {
    public:
        virtual std::vector <Xml::Node*>  operator () (const Xml::Element* context, const Xml::Document& xslDoc, const Xml::Element * applyTemplateElement) const;
    };

    Xml::Document *
    xslTransform(const Xml::Document& xmlDoc, const Xml::Document& xslDoc ) ;

    Xml::Node *
    applyDefaultTemplate(const Xml::Node * context,  const Xml::Document& xslDoc);


    /**
     * Returns the appropriate template (if any) that should be applied
     * to the given element
     *
     * @param xslDoc The XSL document containing the templates
     * @param element The element that the returned template should be applied to
     *
     * @return the template that matches the given element. If no template matches, we return nullptr
     */
    const
    Xml::Element *
    getTemplate(const Xml::Document& xslDoc, const Xml::Element* element);


    std::vector<Xml::Node *>
    findAndApplyTemplate(const Xml::Element * context, const Xml::Document& xslDoc);

    std::vector<Xml::Node *>
    applyTemplate(const Xml::Element *  context, const Xml::Document& xslDoc, const Xml::Element * xslTemplate);

}
#endif //_H_XSL_XSL
