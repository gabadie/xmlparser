#ifndef _H_XML_DOCUMENT
#define _H_XML_DOCUMENT

#include <iosfwd>
#include <string>
#include <vector>

#include "XmlElement.hpp"

namespace Xml
{
    // Forward declarations
    class DocumentNode;

    /**
     * Defines a XML document
     */
    class Document
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
         * Implements standard stream operator
         */
        std::ostream &
        operator >> (std::ostream & stream) const;

        /**
         * Destructor
         */
        virtual
        ~Document();

        /**
         * Gets the root element of the document (non-const version)
         *
         * @return The root element of the document
         */
        Element *
        root();

        /**
         * Gets the root element of the document (const version)
         *
         * @return The root element of the document
         */
        Element const *
        root() const;

        /**
         * Appends a comment to the document
         *
         * @param comment Text of the comment to append
         */
        void
        appendComment(std::string const & comment);

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
         * Appends a DocumentNode to the Document
         *
         * @param documentNode Document node to append
         */
        void
        appendNode(DocumentNode * documentNode);

    protected:
        Element * mRoot;     ///< Root of the XML document
        NodesList mChildren; ///< Children nodes
        //DocType mDocType;  ///< DocType of the XML document //TODO
    };

    /**
     * Defines a sexier standard stream operator
     */
    inline
    std::ostream &
    operator << (std::ostream & stream, Document const & doc)
    {
        return doc >> stream;
    }

}


#endif //_H_XML_DOCUMENT
