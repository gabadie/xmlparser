#ifndef _H_XSD_EXCEPTION
#define _H_XSD_EXCEPTION

#include <stdexcept>
#include <string>

namespace Xsd
{
    /**
     * Define a construction exception
     */
    class XSDConstructionException : public std::runtime_error
    {
    public:
        /**
         * Constructor
         *
         * @param msg The exception message
         */
        XSDConstructionException(const std::string & msg);
    };


    /**
     * Define a validation exception
     */
    class XSDValidationException : public std::runtime_error
    {
    public:
        /**
         * Constructor
         *
         * @param msg The exception message
         */
        XSDValidationException(const std::string & msg);
    };
}

#endif //_H_XSD_EXCEPTION
