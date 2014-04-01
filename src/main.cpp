#include <utility>

#include "Xml/XmlParser.hpp"
#include "Xsl/Xsl.hpp"

static int const SUCCESS = 0x0000;
static int const INVALID_COMMAND = 0x0001;
static int const PARSE_ERROR = 0x0002;

static inline
void
displayMan()
{
    std::cerr << "Available commands are:\n"
              << "xmltool -p file.xml : parse and display the xml file\n"
              << "xmltool -v file.xml file.xsd : parse both xml and xsd files and display the validation result\n"
              << "xmltool -t file.xml file.xsl : parse both xml and xsl files and display de transformation result of file.xml by the stylesheet file.xsl\n"
              << "xmltool -h : displays this help"
              << std::endl;
}

int
appParse(std::string const & xmlPath)
{
    Xml::Log xmlLog;
    Xml::Document * xmlDoc = Xml::load(xmlPath, &xmlLog);

    if(xmlDoc == nullptr)
    {
        std::cerr << "Failed to parse XML file: " << xmlPath << std::endl;
        std::cerr << xmlLog;
        return PARSE_ERROR;
    }

    std::cout << (*xmlDoc) << std::endl;
    std::cerr << xmlLog;

    delete xmlDoc;

    return SUCCESS;
}

int
appVerify(std::string const & xmlPath, std::string const & xsdPath)
{
    (void) xmlPath;
    (void) xsdPath;
    std::cerr << __func__ << " : not implemented yet" << std::endl;
    __builtin_trap();
    return SUCCESS;
}

int
appTransform(std::string const & xmlPath, std::string const & xslPath)
{
    Xml::Log xmlLog;
    Xml::Document * xmlDoc = Xml::load(xmlPath, &xmlLog);

    if(xmlDoc == nullptr)
    {
        std::cerr << "Failed to parse XML file: " << xmlPath << std::endl;
        std::cerr << xmlLog;
        return PARSE_ERROR;
    }


    Xml::Log xslLog;
    Xml::Document * xslDoc = Xml::load(xslPath, &xslLog);

    if(xmlDoc == nullptr)
    {
        std::cerr << "Failed to parse XSL file: " << xmlPath << std::endl;
        std::cerr << xslLog;
        return PARSE_ERROR;
    }

    Xml::Document * transformedDoc = Xsl::xslTransform(*xmlDoc, *xslDoc);

    std::cout << (*transformedDoc) << std::endl;
    //std::cerr << transformLog;

    delete xmlDoc;
    delete xslDoc;
    delete transformedDoc;

    return SUCCESS;
}


template <typename R, typename ...FArgs, typename ...Args>
int
checkArgsAndCall(std::string const & option, int argc, R(* func)(FArgs ...), Args && ...args)
{
    static_assert(sizeof...(FArgs) == sizeof...(Args), "Wrong number of parameters");

    auto nbCmdParams  = argc - 2u; // -1 for executable name, -1 for the option
    auto nbFuncParams = sizeof...(FArgs);

    if(nbCmdParams != nbFuncParams)
    {
        std::cerr << "Invalid number of arguments for option \"" << option << "\" (expected "
                  << nbFuncParams << " got " << nbCmdParams << ")\n" << std::endl;
        displayMan();
        return INVALID_COMMAND;
    }

    return func(std::forward<Args>(args)...);
}

int
main(int argc, char const * const * argv)
{
    if(argc < 2)
    {
        displayMan();
        return INVALID_COMMAND;
    }

    std::string option = argv[1];

    // Ask for help
    if(option == "-h")
    {
        displayMan();
        return SUCCESS;
    }

    // Parse and display the XML file
    if(option == "-p")
    {
        return checkArgsAndCall(option, argc, appParse, argv[2]);
    }

    // Parse both xml and xsd files and display the validation result
    if(option == "-v")
    {
        return checkArgsAndCall(option, argc, appVerify, argv[2], argv[3]);
    }

    // Parse both xml and xsl files and display de transformation result
    if(option == "-t")
    {
        return checkArgsAndCall(option, argc, appTransform, argv[2], argv[3]);
    }

    // If we get here, the command is invalid
    std::cerr << "Invalid command" << std::endl;
    displayMan();

    return INVALID_COMMAND;
}
