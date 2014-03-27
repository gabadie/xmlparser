#ifndef _H_XML_COMMENT
#define _H_XML_COMMENT

#include <iosfwd>
#include <string>

#include "XmlForward.hpp"
#include "XmlDocumentNode.hpp"

namespace Xml
{

    /**
     * Defines a comment
     */
    class Comment final : public DocumentNode
    {
    public:
        /**
         * Destructor
         */
        ~Comment() override;

        /**
         * Override of clone abstract method
         */
        Node *
        clone() const;

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
        void
        exportToStream(std::ostream & stream, std::size_t level,
            std::string const & indent) const override;

    private:
        std::string const mContent;

        friend class Xml::Object;
        friend XML_BISON_MAIN();

    };
}


#endif //_H_XML_COMMENT
