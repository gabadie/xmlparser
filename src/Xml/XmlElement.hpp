#ifndef _H_XML_ELEMENT
#define _H_XML_ELEMENT

#include <map>
#include <string>
#include <vector>

#include "XmlForward.hpp"
#include "XmlDocumentNode.hpp"

namespace Xml
{

    /**
     * Defines an XML element
     */
    class Element final : public DocumentNode
    {
    public:

        // Type aliases
        using AttributesMap = std::map<std::string, std::string>;
        using NodeList      = std::vector<Node *>;
        using ElementList   = std::vector<Element const *>;

        /**
         * Constructor
         *
         * @param name Name of the element
         * @param namespace Name of the namespace
         */
        Element(std::string const & name, std::string const & namespaceName = std::string());

        /**
         * Destructor
         */
        virtual
        ~Element();

        /**
         * Override of clone abstract method
         */
        Node *
        clone();

        /**
         * Gets the children nodes of the element.
         *
         * @return The children nodes of the element
         */
        NodeList const &
        children() const;

        /**
         * Gets the ordered list of the children of the element
         *
         * @return Return a container of Element const *
         */
        ElementList
        elements() const;

        /**
         * Gets the ordered list of the direct children of the element
         * with the given tag
         *
         * @param tag Tag to search for
         *
         * @return Return a container of Element const *
         */
        ElementList
        elements(std::string const & tag) const;

        /**
         * Get the text content of the element
         *
         * @return The text content of the element
         */
        std::string
        text() const;

        /**
         * Get the text content of the element recursively
         *
         * @return The full text content of the element
         */
        std::string
        fullText() const;

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
         * Removes a child node.
         *
         * @return True is the element has been removed, false otherwise.
         */
        bool
        remove(Node * node) override;

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
         * Gets the namespace of the element
         *
         * @return The namespace of the element
         */
        std::string const &
        namespaceName() const;

        /**
         * Set the namespace of the element
         *
         * @param namespace Namespace to set
         */
        void
        setNamespaceName(std::string const & namespaceName);

        /**
         * Gest the namespace name and the name of the element
         *
         * @return the namespace name and the name
         */
        std::string
        tag() const;

        /**
         * Gets the value of an attribute by name
         *
         * @param name Name of the attribute to get.
         *
         * @return The value of the attribute if found, an empty string otherwise.
         */
        std::string const &
        attribute(std::string const & namespaceName) const;

        /**
         * Gets all attributes
         *
         * @return The map of all attributes
         */
        AttributesMap const &
        attributes() const;

        /**
         * Lists namespace's attributes
         *
         * @param namespaceName is the namespace.
         *
         * @return The map of attributes.
         */
        AttributesMap
        namespaceAttributes(std::string const & name) const;

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
         * Returns the element of the XPath query
         *
         * @param xPathQuery XPath query
         *
         * @return The list of Element result of the XPath query
         */
        std::list<Element const *>
        select(std::string const & xPathQuery) const;

        /**
         * Tells whether or not the element matches a given (XPath-like) pattern, typically
         * the "matches" attribute of an XSL template.
         *
         * @param pattern an XPath-like pattern.
         *
         * @return True if the element matches the given pattern, False otherwise.
         */
        bool
        matches(std::string const & pattern) const;

        /**
         * Gets the value of the result of the XPath query
         *
         * @param xPathQuery XPath query
         *
         * @return The value of the result of the XPath query
         */
        std::string
        valueOf(std::string const & xPathQuery) const;

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
        appendNode(Node * node) override;

        /**
         * Tells whether or not the element has the given node in
         * its children recursively.
         *
         * @param node Node to find
         *
         * @return True if found, false otherwise.
         */
        bool
        hasChild(Node const * node) const override;

    protected:
        std::string mName;         ///< Name of the element
        std::string mNamespaceName;    ///< Namespace of the element
        AttributesMap mAttributes; ///< Attributes of the element
        NodeList mChildren;        ///< Children elements

        friend XML_BISON_MAIN();
        friend class Xsl::Instruction;
        friend class Xsl::ValueOf;
        friend class Xsl::ForEach;
        friend class Xsl::ApplyTemplate;
    };
}

#endif //_H_XML_ELEMENT
