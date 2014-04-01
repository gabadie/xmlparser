#ifndef _H_XML_FORWARD
#define _H_XML_FORWARD

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
    int ::yyparse(void ** e)

/*
 * Bison parser declaration
 */
int
yyparse(void ** e);

#endif
