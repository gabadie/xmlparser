#ifndef _H_XML_PARSER_INPUT
#define _H_XML_PARSER_INPUT

#include <iostream>


namespace Xml
{

    void
    flexSetInput(std::istream & inputStream);

    int
    flexInput(char * buffer, int bufferMaxSize);

}

#define YY_INPUT(buffer,buffer_read,buffer_max_size) \
    (buffer_read) = Xml::flexInput(buffer, buffer_max_size);


#endif //_H_XML_PARSER_INPUT
