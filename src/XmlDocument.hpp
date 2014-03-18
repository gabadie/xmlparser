#ifndef _H_XML_DOCUMENT
#define _H_XML_DOCUMENT

#include <iosfwd>
#include <string>
#include <vector>

#include "XmlElement.hpp"


namespace Xml
{
    // Forward declarations
    class MiscNode;

    /**
     * Defines a XML document
     */
    class Document
    {
    public:

        using MiscNodesList = std::vector<MiscNode *>;

        /**
         * Constructor
         *
         * @param root Root element of the document
         */
        Document(Element * root = nullptr);

        /**
         * Implements standard stream operator
         */
        inline
        std::ostream &
        operator >> (std::ostream & stream) const
        {
            exportToStream(stream, "");
            return stream;
        }

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
         * Sets the root element of the document
         *
         * @param root New root of the document
         */
        void
        setRoot(Element * root);

        /**
         * Saves the XML document into a file.
         *
         * @param path File path
         */
        bool
        saveToFile(std::string const & path) const;

        /**
         * Loads a document from a file
         *
         * @param path Path of the file to load the document from
         *
         * @return A new XML document if successful, nullptr otherwise.
         */
        static
        Document *
        loadFromFile(std::string const & path);

    protected:
        /**
         * Exports to a <stream> with a given <indent>
         *
         * @param <stream> is the stream to export to
         * @param <indent> is the the indentation prefix
         */
        virtual
        void
        exportToStream(std::ostream & stream, std::string const & indent) const;

    protected:
        Element * mRoot;     ///< Root of the XML document
        MiscNodesList mMisc; ///< Misc nodes (comments & PI)
        //DocType mDocType;    ///< DocType of the XML document //TODO
    };
}


#endif //_H_XML_DOCUMENT
