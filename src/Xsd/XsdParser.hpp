#ifndef _H_XSD_PARSER
#define _H_XSD_PARSER

//TODOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
//check si le xsd du document est ok pour le doc xml

#include <string>
#include <map>

#include "XsdType.hpp"
#include "XsdAttribute.hpp"

namespace Xsd
{
    class Type;
    class Attribute;

    //Aliases
    using typesMap = std::map<std::string, Type const *>;
    using elementsTypesMap = std::map<std::string, std::string>;
    using attributesMap = std::map<std::string, Attribute const *>;
    using attributesTypesMap = std::map<std::string, std::string>;

    /**
     * Defines the schema of an XML Document and allow its validation
     */
    class Document
    {
        public :
            /**
             * Constructor
             */
            Document(std::string xsdFilePath);

            /**
             * Destructor
             */
            virtual ~Document();

        protected:
            std::string mFilePath;
            typesMap mTypes;
            elementsTypesMap mElementsTypes;
            attributesMap mAttributes;
            attributesTypesMap mAttributesTypes;
    };
}

#endif //_H_XSD_DOCUMENT