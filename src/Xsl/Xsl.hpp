#ifndef _H_XSL_XSL
#define _H_XSL_XSL

#include "../Xml/XmlForward.hpp"


namespace Xsl
{

    /**
     * Transform an <xmlDoc> into a new document Xml applying a style sheet <xslDoc> 
     * @param <xmlDoc> The document Xml to transform
     * @param <xslDoc> style sheet Xsl used to apply the transformation 
     * @return TNew Document XML transformed
     */
    Xml::Document *
    xslTransform(Xml::Document const & xmlDoc, Xml::Document const & xslDoc);

}

#endif //_H_XSL_XSL
