#ifndef _H_XSD_ATTRIBUTE
#define _H_XSD_ATTRIBUTE

#include <string>
#include "../Xml/XmlElement.hpp"

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
            Attribute(const std::string & name, bool required, const std::string & typeName, bool ref);

            static Attribute *
            parseAttribute(const Xml::Element * const xmlElement);

            /**
             * Destructor
             */
            virtual ~Attribute() {}

            std::string
            name();

            bool
            isRequired();

        protected:
            std::string mName;
            bool mRequired;

            void
            init(const std::string & name, bool required, const std::string & typeName, bool ref);

            void
            checkValidity(const std::string & value);

    };
}

#endif //_H_XSD_ATTRIBUTE