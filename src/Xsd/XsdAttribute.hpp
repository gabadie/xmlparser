#ifndef _H_XSD_ATTRIBUTE
#define _H_XSD_ATTRIBUTE

#include <string>

#include "XsdType.hpp"

namespace Xsd
{
    class Type;

    /**
     * Defines the schema of an XML attribute
     */
    class Attribute
    {
        public:
            /**
             * Constructor
             */
            Attribute(std::string const & name, bool required);

            /**
             * Destructor
             */
            virtual ~Attribute();

            Attribute
            parseAttribute(Xml::Element xmlElement, std::map<std::string, Attribute const *> attributesMap, std::map<std::string, std::string> attributesTypesMap) const;

        protected:
            std::string mName;
            bool mRequired;
    };
}

#endif //_H_XSD_ATTRIBUTE