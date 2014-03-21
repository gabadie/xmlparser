#ifndef _H_XML_COMMENT
#define _H_XML_COMMENT

#include <iosfwd>
#include <string>

#include "XmlDocumentNode.hpp"

namespace Xml
{
    // Forward declarations
    class Document;
    class Element;
    class Test;

    /**
     * Defines a comment
     */
    class Comment : public DocumentNode
    {
    public:

        /**
         * Destructor
         */
        virtual
        ~Comment();

    protected:
        /**
         * Constructor
         *
         * @param content Content of the comment
         */
        Comment(std::string const & content = "");

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
        std::string mContent;

        friend class Xml::Document;
        friend class Xml::Element;
        friend class Xml::Test;

    };
}


#endif //_H_XML_COMMENT
