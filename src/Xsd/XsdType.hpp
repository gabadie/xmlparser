#ifndef _H_XSD_TYPE
#define _H_XSD_TYPE

#include <string>

namespace Xsd
{
    class Type
    {
    public:
         /**
         * Destructor
         */
        virtual
        ~Type();

    protected:
        /**
         * Constructor
         */
        Type();

        /**
         * Create the type regex
         */
        void
        createRegex();

    protected:
        std::string mRegex;

    }
}