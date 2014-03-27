#ifndef _H_XML_TEXT
#define _H_XML_TEXT

#include <iosfwd>
#include <string>

#include "XmlForward.hpp"
#include "XmlNode.hpp"

namespace Xml
{

    /**
     * Defines a text node
     */
    class Text final : public Node
    {
    public:

        /**
         * Destructor
         */
        ~Text() override;

        /**
         * Override of clone abstract method
         */
        Node *
        clone() const ;

    protected:
        /**
         * Constructor
         *
         * @param content Text content
         */
        Text(std::string const & content = "");

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
         * Gets the content text
         *
         * @return The content text
         */
        std::string const &
        contentText() const override;

    private:
        std::string const mText; ///< Text content

        friend class Xml::Element;
        friend XML_BISON_MAIN();
    };
}


#endif //_H_XML_TEXT
