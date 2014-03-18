#ifndef _H_XML_ELEMENT
#define _H_XML_ELEMENT

#include <deque>
#include <map>
#include <string>
#include <vector>

#include "XmlNode.hpp"

namespace Xml
{
    //Forward declarations
    class MiscNode;

    /**
     * Defines an XML element
     */
    class Element : public Node
    {
    public:

        // Type aliases
        using AttributesMap = std::map<std::string, std::string>;
        using NodeList      = std::deque<Node *>;
        using ElementList   = std::vector<Element const *>;

        /**
         * Constructor
         *
         * @param name Name of the element
         * @param parent Parent of the element
         */
        Element(std::string const & name, Node * parent = nullptr);

        /**
         * Destructor
         */
        virtual
        ~Element();

        /**
         * Gets the ordered list of the children of the element
         *
         * @return Return a container of Element const *
         */
        ElementList
        elements() const;

        /**
         * Get the parent element of the element
         *
         * @return The parent element if found, nullptr otherwise.
         */
        Element const *
        parentElement() const;

        /**
         * Get the text content of the element
         *
         * @return The text content of the element
         */
        std::string
        text() const;

        /**
         * Set the content of the element
         *
         * Delete all children nodes and add a text node with
         * the given content.
         *
         * @param content New text content
         */
        void
        setContent(std::string const & content);

        /**
         * Discard the content of the element.
         *
         * All the children are deleted.
         */
        void
        clearContent();

        /**
         * Appends an element child
         *
         * @param element Child element to append
         */
        void
        append(Element * element);

        /**
         * Add a text node to the element.
         *
         * @param text Text content of the text node
         */
        void
        appendText(std::string const & text);

        /**
         * Appends a comment to the element
         *
         * @param comment Text of the comment to append
         */
        void
        appendComment(std::string const & comment);

        /**
         * Appends a PI to the element
         *
         * @param pi Text of the PI to append
         */
        void
        appendPI(std::string const & pi);

        /**
         * Gets the name of the element
         *
         * @return The name of the element
         */
        std::string const &
        name() const;

        /**
         * Set the name of the element
         *
         * @param name Name to set
         */
        void
        setName(std::string const & name);

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
         * @param stream The stream to export to
         * @param level  Level of the token
         * @param indent The indentation prefix
         */
        virtual
        void
        exportToStream(std::ostream & stream, std::size_t level,
            std::string const & indent) const override;

    private:

        /**
         * Tells whether or not the node is an Element
         *
         * @return True if the node is an Element, false otherwise.
         */
        virtual
        bool
        isElement() const override;

        /**
         * Appends a node
         *
         * @param node Node to append
         */
        void
        appendNode(Node * node);

    protected:
        std::string mName;         ///< Name of the element
        AttributesMap mAttributes; ///< Attributes of the element
        NodeList mChildren;        ///< Children elements
    };
}

#endif //_H_XML_ELEMENT
