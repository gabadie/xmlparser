#ifndef _H_XML_FORWARD
#define _H_XML_FORWARD

#include <vector>


namespace Xml
{

    /*
     * Namespace's classes
     */
    class CharacterData;
    class Comment;
    class Doctype;
    class Document;
    class DocumentNode;
    class Element;
    class Log;
    class Node;
    class Object;
    class ProcessingInstruction; class Text;

    /*
     * XML object's type label
     */
    enum class ObjectLabel
    {
        CharacterData,
        Comment,
        Doctype,
        Document,
        Element,
        ProcessingInstruction,
        Text
    };

}

namespace Xsl
{

    /*
     * Xsl namespace's classes
     */
    class ApplyTemplate;
    class ForEach;
    class Instruction;
    class ValueOf;

}

/*
 * Bison parser
 */
#define XML_BISON_MAIN() \
    int ::yyparse(void ** e);


/*
 * Bison parser declaration
 */
int
yyparse(void ** e);

/*
#define XSL_APPLY_TEMPLATE() \
    void applyTemplate (Xml::Node* context, std::vector<Xml::Node*> &listNodes, Xml::Document& xslDoc, Xml::Element& xslTemplate);
#define XSL_APPLY_DEFAULT_TEMPLATE() \
    Xml::Node* applyDefaultTemplate(Xml::Node* context, Xml::Document& xslDoc);*/

#endif
