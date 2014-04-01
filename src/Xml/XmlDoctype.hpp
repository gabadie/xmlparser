#ifndef _H_DOCTYPE
#define _H_DOCTYPE

#include <string>

#include "XmlForward.hpp"
#include "XmlDocumentNode.hpp"

namespace Xml
{

    /**
     * Defines a doctype
     */
    class Doctype final : public DocumentNode
    {
    public:
        /**
         * Destructor
         */
        ~Doctype() override;

        /**
         * Override of clone abstract method
         */
        Node *
        clone();

        /**
         * Returns the object's type label
         *
         * @return The object's type label
         */
        virtual
        ObjectLabel
        objectLabel() const;

    protected:
        /**
         * Constructor
         *
         * @param content Content of the comment
         */
        Doctype(std::string const & content = "");

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

#endif //_H_DOCTYPE
