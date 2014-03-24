#include "XsdAttribute.hpp"
#include "XsdChecker.hpp"

#ifdef APP_DEBUG
#include <cassert>
#endif

namespace Xsd
{
    Attribute::Attribute(std::string const & name, bool required):
        mName(name),
        mRequired(required)
    {

    }

    Attribute::~Attribute()
    {

    }

    Attribute
    Attribute::parseAttribute(Xml::Element xmlElement)
    {
        bool required = false;
        std::string attributeName;

        std::string name = xmlElement.attribute("name");
        std::string ref = xmlElement.attribute("ref");
        std::string type = xmlElement.attribute("type");
        std::string use = xmlElement.attribute("use");

        if(name != "")
        {
            attributeName = name;
            // Add attribute to attribute type map
            if(type != "")
            {
                attributesTypesMap.insert(std::pair<std::string,std::string>(attributeName, type));
            }
            else
            {
                // TODO Gestion exception
                std::cout << "Error : Attribute element doesn't have any type attribute" << std::endl;
            }
        }
        else if(ref != "")
        {
            attributeName = ref;
        }
        else
        {
            // TODO Gestion exception
            name = "";
            std::cout << "Error : Attribute element doesn't have any name attribute" << std::endl;
        }

        if(use == "required")
        {
            required = true;
        }


        Attribute attr(attributeName, required);

        if(name != "") {
            // Add attribute to the attribute map
            attributesMap.insert(std::pair<std::string, Attribute const *>(attributeName, &attr));
        }

        return attr;
    }
}