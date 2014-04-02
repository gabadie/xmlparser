
#include "Xsl.hpp"
#include "XslTemplate.hpp"

#include "../Xml/XmlDocument.hpp"
#include "../Xml/XmlNode.hpp"
#include "../Xml/XmlText.hpp"

#include "../AppDebug.hpp"


Xml::Document *
Xsl::xslTransform(Xml::Document const & xmlDoc, Xml::Document const & xslDoc)
{
    Xml::Document * result = new Xml::Document();
    std::vector<Xml::Node *> resultNodes = findAndApplyTemplate(xmlDoc.root(), xslDoc);

    if (resultNodes.size() == 0)
    {
        return result;
    }

    app_assert(resultNodes[0]->isElement());

    auto root = static_cast<Xml::Element *>(resultNodes[0]);
    result->setRoot(root);

    return result;
}
