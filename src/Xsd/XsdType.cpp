#include "XsdType.hpp"


namespace Xsd
{
    Type::Type()
    {
        mRegex = "";
    }

    Type::~Type()
    {

    }

    void
    Type::parseTypeFromComplexType(Xml::Element xmlElement)
    {

    }

    std::string
    Type::getRegexFromElement(Xml::Element xmlElement)
    {
        std::string name = xmlElement.attribute("name");
        std::string ref = xmlElement.attribute("ref");
        std::string min = xmlElement.attribute("minOccurs");
        std::string max = xmlElement.attribute("maxOccurs");

        std::string regex;
        std::string attributeName;
        std::string minOccurs = "1";
        std::string maxOccurs = "*";

        // Get the element name
        if(name != "")
        {
            attributeName = name;
        }
        else if(ref != "")
        {
            attributeName = ref;
        }
        else
        {
            // TODO Gestion exception
            attributeName = "";
            std::cout << "Error : Attribute element doesn't have any name attribute" << std::endl;
        }

        // Get the occurence
        if(min != "")
        {
            minOccurs = min;
        }
        if(max != "")
        {
            maxOccurs = max;
        }

        // Create the regex
        regex = "<(" + attributeName + ">){" + minOccurs + "})((<" + attributeName + ">?){" + maxOccurs + "}";
        return regex;
    }
}
