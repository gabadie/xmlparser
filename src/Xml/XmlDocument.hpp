#ifndef _H_XML_DOCUMENT
#define _H_XML_DOCUMENT

#include <iosfwd>
#include <string>

#include "XmlForward.hpp"
#include "XmlObject.hpp"
#include "XmlElement.hpp"

namespace Xml
{

    /**
     * Defines a XML document
     */
    class Document final : public Object
    {
    public:

        /**
         * Constructor
         *
         * @param root Root element of the document
         */
        Document(Element * root = nullptr);

        /**
         * Destructor
         */
        ~Document() override;

        /**
         * Override of Xml::Object::objectLabel()
         */
        ObjectLabel
        objectLabel() const override;

        /**
         * Gets the itself document (const version)
         *
         * @return The document
         */
        Document const *
        document() const override final;

        /**
         * Gets the parent xml object (const version)
         *
         * @return nullptr
         */
        Object const *
        parent() const override final;

        /**
         * Gets the document's root
         *
         * @return The document root
         */
       /* Element *
        root()
        {
            return mRoot;
        }
*/
        Element const *
        root() const
        {
            return mRoot;
        }

        /**
         * Removes a child node.
         *
         * @return True is the element has been removed, false otherwise.
         */
        bool
        remove(Node * node) override;

        /**
         * Sets the root element of the document
         *
         * @param root New root of the document
         */
        void
        setRoot(Element * root);

        /**
         * Gets the children nodes of the document
         *
         * @return The children nodes of the document
         */
        NodeList const &
        children() const;

        /**
         * Saves the XML document into a file.
         *
         * @param path File path
         *
         * @return True if the file has been saved, false otherwise.
         */
        bool
        saveToFile(std::string const & path) const;

    protected:
        /**
         * Exports to a <stream> with a given <indent>
         *
         * @param stream The stream to export to
         * @param level  Level of the token
         * @param indent The indentation prefix
         */
        void
        exportToStream(std::ostream & stream, std::size_t level,
            std::string const & indent) const override;

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

        /**
         * Tests if can append this node to the document
         *
         * @return True if you can append the node
         */
        static bool
        canAppend(Node const * node);

    private:
        Element * mRoot;     ///< Root of the XML document
        NodeList mChildren; ///< Children nodes

        friend
        Xml::Document *
        Xsl::transform(Xml::Document const &, Xml::Document const &, Xml::Log &);
        friend XML_BISON_MAIN();
    };

}

#endif //_H_XML_DOCUMENT
