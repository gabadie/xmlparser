#ifndef _H_CHARACTER_DATA
#define _H_CHARACTER_DATA

#include <string>

#include "XmlForward.hpp"
#include "XmlNode.hpp"

namespace Xml
{

    /**
     * Defines a comment
     */
    class CharacterData final : public Node
    {
    public:
        /**
         * Destructor
         */
        ~CharacterData() override;

        /**
         * Override of Xml::Object::objectLabel()
         */
        ObjectLabel
        objectLabel() const override;

        /**
         * Override of clone abstract method
         */
        Node *
        clone() const override;

    protected:
        /**
         * Constructor
         *
         * @param content Content of the comment
         */
        CharacterData(std::string const & content = "");

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
