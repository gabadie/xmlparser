#ifndef _H_XML_DOCUMENT
#define _H_XML_DOCUMENT

#include <iosfwd>
#include <string>
#include <vector>

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

        using NodesList = std::vector<DocumentNode *>;

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
        Element *
        root()
        {
            return mRoot;
        }

        Element const *
        root() const
        {
            return mRoot;
        }

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
        NodesList const &
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

    protected:
        Element * mRoot;     ///< Root of the XML document
        NodesList mChildren; ///< Children nodes
        //DocType mDocType;  ///< DocType of the XML document //TODO

        friend XML_BISON_MAIN();
    };

}

#endif //_H_XML_DOCUMENT
