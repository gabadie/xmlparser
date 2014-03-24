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
            Attribute(const std::string & name, bool required);

            /**
             * Destructor
             */
            virtual ~Attribute();

            Attribute
            parseAttribute(const Xml::Element & xmlElement) const;

        protected:
            std::string mName;
            bool mRequired;
    };
}

#endif //_H_XSD_ATTRIBUTE