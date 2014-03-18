#ifndef _H_XML_TEXT
#define _H_XML_TEXT

#include <iosfwd>
#include <string>

#include "XmlNode.hpp"

namespace Xml
{
    /**
     * Defines a text node
     */
    class Text : public Node
    {
    public:

        /**
         * Constructor
         *
         * @param content Text content
         */
        Text(std::string const & content = "");

        /**
         * Destructor
         */
        virtual
        ~Text();

        /**
         * Set the text content
         *
         * @param text Text content to set
         */
        void
        setText(std::string const & text);

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
         * Gets the content text
         *
         * @return The content text
         */
        virtual
        std::string const &
        contentText() const override;

    protected:
        std::string mText; ///< Text content
    };
}


#endif //_H_XML_TEXT
