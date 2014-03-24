#include "XsdAttribute.hpp"
#include "XsdDocument.hpp"

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

    Attribute::parseAttribute(Xml::XmlElement xmlElement):
    {
        bool required = false;
        std::string name;
        map<std::string,std::string>::iterator itName = xmlElement.mAttributes.find('name');
        map<std::string,std::string>::iterator itRef = xmlElement.mAttributes.find('ref');
        map<std::string,std::string>::iterator itType = xmlElement.mAttributes.find('type');
        map<std::string,std::string>::iterator itRequired = xmlElement.mAttributes.find('use');

        bool hasName, hasRef, hasType, hasRequired;
        hasName = (itName != xmlElement.mAttributes.end());
        hasRef = (itRef != xmlElement.mAttributes.end());
        hasType = (itType != xmlElement.mAttributes.end());
        hasRequired = (itRequired != xmlElement.mAttributes.end());


        if(hasName)
        {
            name = (*itName).second;
            // Add attribute to attribute type map
            if(hasType)
            {
                Xsd::attributesTypesMap.insert(std::pair<std::string,std::string>(name, (*itType).second);
            }
            else
            {
                // TODO Gestion exception
                cout << "Error : Attribute element doesn't have any type attribute" << endl;
            }
        }
        else if(hasRef)
        {
            name = (*itRef).second;
        }
        else
        {
            // TODO Gestion exception
            name = "";
            cout << "Error : Attribute element doesn't have any name attribute" << endl;
        }

        if(hasRequired)
        {
            if((*itRequired).seconde == "required")
            {
                required = true;
            }
        }

        Attribute attr(name, required);

        if(hasName) {
            // Add attribute to the attribute map
            Xsd::attributesMap.insert(std::pair<std::string, Attribute const *>(name,attr));
        }

        return attr;
    }
}