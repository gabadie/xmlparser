#ifndef _H_XML_COMMENT
#define _H_XML_COMMENT

#include <iosfwd>
#include <string>

#include "XmlMiscNode.hpp"
namespace Xml
{
    /**

     * Defines a comment
     */
    class Comment : public MiscNode
    {
    public:

        /**
         * Constructor
         *
         * @param content Content of the comment
         * @param parent Parent of the node
         */
        Comment(std::string const & content = "", Node * parent = nullptr);

        /**
         * Destructor
         */
        virtual
        ~Comment();

    protected:
        /**
         * Exports to a <stream> with a given <indent>
         *
         * @param <stream> is the stream to export to
         * @param <indent> is the the indentation prefix
         */
        virtual
        void
        exportToStream(std::ostream & stream, std::string const & indent) const override;

        /**
         * Gets the content text if is a Xml::Text
         */
        virtual
        std::string const &
        contentText() const;

    protected:
        std::string mContent;
    };
}


#endif //_H_XML_COMMENT
