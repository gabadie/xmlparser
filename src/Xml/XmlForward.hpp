#ifndef _H_XML_FORWARD
#define _H_XML_FORWARD

namespace Xml
{

    /*
     * Namespace's classes
     */
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
