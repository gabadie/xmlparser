#ifndef _H_XSL_XSL
#define _H_XSL_XSL

#include "../Xml/XmlForward.hpp"


namespace Xsl
{


    bool 
    XslCheckValidity(Xml::Document const & xslDoc, Xml::Log & xslLog);

    /**
     * Transform an <xmlDoc> into a new document Xml applying a style sheet <xslDoc>
     * @param <xmlDoc> The document Xml to transform
     * @param <xslDoc> style sheet Xsl used to apply the transformation
     * @return TNew Document XML transformed
     */
    Xml::Document *
    transform(Xml::Document const & xmlDoc, Xml::Document const & xslDoc, Xml::Log & xslLog);

}

#endif //_H_XSL_XSL
