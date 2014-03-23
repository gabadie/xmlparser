#ifndef _H_XML_OBJECT
#define _H_XML_OBJECT

#include <iostream>


namespace Xml
{
    // Forward declarations
    class Document;
    class Element;
    class Node;
    class ProcessingInstruction;
    class Test;

    /**
     * Defines a abstract interface for all Xml classes
     */
    class Object
    {
    public:
        /**
         * Constructor
         */
        Object() = default;

        /**
         * Destructor
         */
        virtual
        ~Object();

        /**
         * Deletes copy constructor and operator =
         */
        Object(Object const &) = delete;

        void
        operator = (Object const &) = delete;

        /**
         * Gets the object's document
         *
         * @return The document
         */
        Document *
        document();

        virtual
        Document const *
        document() const = 0;

        /**
         * Gets the object's parent
         *
         * @return The parent (either a Document or an Element)
         */
        Object *
        parent();

        virtual
        Object const *
        parent() const = 0;

        /**
         * Implements standart stream operator
         */
        inline
        std::ostream &
        operator >> (std::ostream & stream) const
        {
            exportToStream(stream, 0, "  ");
            return stream;
        }

        /**
         * Appends a comment to the element
         *
         * @param comment Text of the comment to append
         */
        void
        appendComment(std::string const & comment);

        /**
         * Appends a processing instruction (PI) to the element
         *
         * @param name Name of the PI to append
         * @param ...keyValues Key and values parameters of the PI
         */
        template <typename ...KeyValues>
        void
        appendProcessingInstruction(std::string const & name, KeyValues && ...keyValues);

        /**
         * Appends a processing instrcution (PI) to the document
         *
         * @param pi The PI to append
         */
        void
        appendProcessingInstruction(ProcessingInstruction * pi);

    protected:
        friend class Xml::Element;

        /**
         * Exports to a <stream> with a given <indent>
         *
         * @param stream The stream to export to
         * @param level  Level of the token
         * @param indent The indentation prefix
         */
        virtual
        void
        exportToStream(std::ostream & stream, std::size_t level,
            std::string const & indent) const = 0;

        /**
         * Tells whether or not the node is an Element
         *
         * @return True if the node is an Element, false otherwise.
         */
        virtual
        bool
        isElement() const;

        /**
         * Appends a node
         *
         * @param node Node to append
         */
        virtual
        void
        appendNode(Node * node);

    protected:
        friend class Xml::Test;
    };

    /**
     * Defines a sexier standart stream operator
     */
    inline
    std::ostream &
    operator << (std::ostream & stream, Object const & object)
    {
        return object >> stream;
    }

}

/*
 * #include "XmlObject.inl" is included in XmlProcessingInstruction.inl to
 * avoid header circular dependencies
 */
//#include "XmlObject.inl"

#endif //_H_XML_OBJECT
