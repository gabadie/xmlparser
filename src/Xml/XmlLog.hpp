#ifndef _H_XML_LOG
#define _H_XML_LOG

#include <iostream>
#include <list>
#include <string>

namespace Xml
{

    /**
     * Defines Xml log
     */
    class Log
    {
    public:

        /**
         * Constructor
         *
         * @param parent Parent of the node
         */
        Log();

        /**
         * Destructor
         */
        ~Log();

        /**
         * Implements standart stream operator
         */
        std::ostream &
        operator >> (std::ostream & stream) const;

        /**
         * Appends a line in the Xml log
         *
         * @param line is the line to append to the log's tail.
         */
        void
        append(std::string const & line);

        /**
         * Checks a substring in the Xml log
         *
         * @param line is the line to append to the log's tail.
         *
         * @return the first line if found or 0 if not found
         */
        size_t
        find(std::string const & subString) const;

    protected:
        std::list<std::string> mLines; ///< List of lines
    };

    /**
     * Defines a sexier standart stream operator
     */
    inline
    std::ostream &
    operator << (std::ostream & stream, Log const & log)
    {
        return log >> stream;
    }

}


#endif //_H_XML_NODE
