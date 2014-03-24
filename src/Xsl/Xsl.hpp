#include <vector>
#include <map>

#include "../Xml/XmlNode.hpp"
#include "../Xml/XmlText.hpp"

namespace Xsl {

    class Instruction
    {
        virtual void operator () (Xml::Element * xslElement, Xml::Node* XmlContext);
    };

    std::map<std::string, Instruction*> xslInstructions;

    Xml::Document xslTransform(Xml::Document& xmlDoc, Xml::Document& xslDoc );

    std::vector <Xml::Node*> applyDefaultTemplate(Xml::Node* context, std::vector<Xml::Node*> resultNodes);

    std::vector <Xml::Node*> applyTemplate (const Xml::Element& xslTemplate, const Xml::Node* context, std::vector<Xml::Node*> resultNodes);
}