#include <vector>

#include "../Xml/XmlNode.hpp"

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