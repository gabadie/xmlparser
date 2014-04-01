#ifndef _H_XML_FORWARD
#define _H_XML_FORWARD
#include <vector>

#define private public
#define protected public 
//TODO

namespace Xml
{

    /*
     * Namespace's classes
     */
    class CharacterData;
    class Comment;
    class Document;
    class DocumentNode;
    class Element;
    class Log;
    class Node;
    class Object;
    class ProcessingInstruction;
    class Text;

}

namespace Xsl
{

    /*
     * Namespace's classes
     */
    class Instruction;
    class ValueOf;
    class ForEach;
    class ApplyTemplate;



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
