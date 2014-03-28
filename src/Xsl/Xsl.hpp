#ifndef _H_XSL_XSL
#define _H_XSL_XSL

#include <vector>
#include <map>
using namespace std;

#include "../Xml/XmlNode.hpp"
#include "../Xml/XmlText.hpp"
#include "../Xml/XmlDocument.hpp"


typedef vector< Xml::Node*> nodeVector;

namespace Xsl {

    class Instruction
    {
        public:
        virtual vector <Xml::Node*>  operator () (const Xml::Node* context, Xml::Document& xslDoc,  const Xml::Element * forEachElement) const = 0;
    };


    class ValueOf : public Instruction
    {
        public:
        virtual vector <Xml::Node*>  operator () (const Xml::Node* context,  Xml::Document& xslDoc,  const Xml::Element * xslElement) const;
    };

    class ForEach: public Instruction
    {
        public:
        virtual vector <Xml::Node*>  operator () (const Xml::Node* context, Xml::Document& xslDoc,  const Xml::Element * forEachElement) const;
    };

    class ApplyTemplate: public Instruction
    {
        public:
        virtual vector <Xml::Node*>  operator () (const Xml::Node* context, Xml::Document& xslDoc, const Xml::Element * applyTemplateElement) const;
    };

    Xml::Document *
    xslTransform(Xml::Document& xmlDoc, Xml::Document& xslDoc ) ;

    Xml::Node * applyDefaultTemplate( const Xml::Node * context,  Xml::Document& xslDoc, Xml::Document& xmlDoc);


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
    getTemplate(Xml::Document& xslDoc, const Xml::Element* element);

    std::vector<Xml::Node *> applyTemplate(const Xml::Node * context, Xml::Document& xslDoc,const Xml::Element * xslTemplate,  Xml::Document& xmlDoc);

    //void applyTemplate ( Xml::Node* context, Xml::Document& xslDoc, vector< Xml::Node*> &resultNodes, Xml::Element * xslTemplate) ;
}
#endif //_H_XSL_XSL
