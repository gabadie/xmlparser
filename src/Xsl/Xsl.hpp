#ifndef _H_XSL_XSL
#define _H_XSL_XSL

#include "../Xml/XmlForward.hpp"


namespace Xsl
{

    /**
     * TODO: doc
     */
    Xml::Document *
    xslTransform(Xml::Document const & xmlDoc, Xml::Document const & xslDoc);

}

#endif //_H_XSL_XSL
