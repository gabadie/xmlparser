#ifndef _H_XML_PROCESSING_INSTRUCTION
#define _H_XML_PROCESSING_INSTRUCTION

#include <iosfwd>
#include <string>
#include <map>

#include "XmlDocumentNode.hpp"

namespace Xml
{
    // Forward declarations
    class Document;
    class Element;

    /**
     * Defines a processing instruction
     */
    class ProcessingInstruction : public DocumentNode
    {
    public:
        // Type aliases
        using AttributesMap = std::map<std::string, std::string>;

        /**
         * Constructor
         *
         * @param content Content of the comment
         */
        ProcessingInstruction(std::string const & name = "");

        /**
         * Destructor
         */
        virtual
        ~ProcessingInstruction();

        /**
         * Gets the value of an attribute by name
         *
         * @param name Name of the attribute to get.
         *
         * @return The value of the attribute if found, an empty string otherwise.
         */
        std::string const &
        attribute(std::string const & name) const;

        /**
         * Sets the value of an attribute.
         *
         * If the attribute does not exist, it is created.
         *
         * @param name  Name of the attribute to set
         * @param value Value to set
         */
        void
        setAttribute(std::string const & name, std::string const & value);

    protected:
        /**
         * Exports to a <stream> with a given <indent>
         *
         * @param <stream> is the stream to export to
         * @param <indent> is the the indentation prefix
         */
        virtual
        void
        exportToStream(std::ostream & stream, std::size_t level,
            std::string const & indent) const override;

    protected:
        std::string mName;         ///< Name of the PI
        AttributesMap mAttributes; ///< Attributes of the element

        friend class Xml::Document;
        friend class Xml::Element;
    };
}


#endif //_H_XML_PROCESSING_INSTRUCTION
