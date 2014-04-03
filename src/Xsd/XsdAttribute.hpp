#ifndef _H_XSD_ATTRIBUTE
#define _H_XSD_ATTRIBUTE

#include <string>
#include "../Xml/XmlElement.hpp"

namespace Xsd
{
    class Type;
    class Checker;
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
             * Creates an attribute with an xml parsed element
             *
             * @param xmlElement The xml element
             * @param checker The checker
             *
             * @return The attribute created
             */
            static Attribute *
            parseAttribute(Xml::Element const * const xmlElement, Checker * checker);

            /**
             * Destructor
             */
            virtual ~Attribute();

            /**
             * Gets the name of the attribute
             *
             * @return The name of the attribute
             */
            std::string
            name();

            /**
             * Indicates if the attribute is required or not
             *
             * @return True if the attribute is required, false otherwise
             */
            bool
            isRequired();

        protected:
            std::string mName;          /// the name of the attribute
            bool mRequired;             /// indicates if the attribute is required or not

            /**
             * Initialise the attribute
             *
             * @param name The name of the attribute
             * @param required Indicates if the attribute is required or not
             * @param typeName the type name of the attribute
             * @param ref Indicates if this is a reference or not
             * @param checker The checker
             */
            void
            init(std::string const & name, bool required, std::string const & typeName, bool ref, Checker * checker);

            /**
             * Checks the validity of the value of an attribute
             *
             * @param value The value to check
             * @param checker The checker
             */
            void
            checkValidity(std::string const & value, Checker * checker);

    };
}

#endif //_H_XSD_ATTRIBUTE
