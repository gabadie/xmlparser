#ifndef _H_XML_PROCESSING_INSTRUCTION
#define _H_XML_PROCESSING_INSTRUCTION

#include <iosfwd>
#include <string>
#include <map>

#include "XmlForward.hpp"
#include "XmlDocumentNode.hpp"

namespace Xml
{

    /**
     * Defines a processing instruction
     */
    class ProcessingInstruction final : public DocumentNode
    {
    public:
        // Type aliases
        using AttributesMap = std::map<std::string, std::string>;

        /**
         * Destructor
         */
        ~ProcessingInstruction() override;

        /**
         * Gets the value of an attribute by name
         *
         * @param name Name of the attribute to get.
         *
         * @return The value of the attribute if found, an empty string otherwise.
         */
        std::string const &
        attribute(std::string const & name) const;

    protected:
        /**
         * Variadic constructor
         *
         * @param name Name of the PI to append
         * @param ...keyValues Key and values parameters of the PI
         */
        template <typename ...KeyValues>
        ProcessingInstruction(std::string const & name, KeyValues && ...keyValues);

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

        /**
         * Sets the value of an attribute.
         *
         * If the attribute does not exist, it is created.
         *
         * @param name  Name of the attribute to set
         * @param value Value to set
         */
        inline
        void
        setAttribute(char const * name, char const * value)
        {
            setAttribute(std::string(name), std::string(value));
        }

        /**
         * Exports to a <stream> with a given <indent>
         *
         * @param <stream> is the stream to export to
         * @param <indent> is the the indentation prefix
         */
        void
        exportToStream(std::ostream & stream, std::size_t level,
            std::string const & indent) const override;

    private:
        /**
         * Sets the value of an attribute (variadic version)
         *
         * If the attribute does not exist, it is created.
         *
         * @param name  Name of the attribute to set
         * @param value Value to set
         * @param ...keyValues Other key values to set
         */
        template <typename ...KeyValues>
        void
        setAttribute(std::string const & name, std::string const & value,
            KeyValues && ...keyValues);

        /**
         * Sets the value of an attribute (variadic version)
         *
         * If the attribute does not exist, it is created.
         *
         * @param name  Name of the attribute to set
         * @param value Value to set
         * @param ...keyValues Other key values to set
         */
        template <typename ...KeyValues>
        void
        setAttribute(char const * name, char const * value,
            KeyValues && ...keyValues);

    private:
        std::string mName;         ///< Name of the PI
        AttributesMap mAttributes; ///< Attributes of the element

        friend class Xml::Object;
        friend XML_BISON_MAIN();
    };
}

#include "XmlProcessingInstruction.inl"

#endif //_H_XML_PROCESSING_INSTRUCTION
