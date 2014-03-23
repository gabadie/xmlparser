#include "XsdChecker.hpp"
#include "../Xml/XmlParser.hpp"

#ifdef APP_DEBUG
#include <cassert>
#endif

namespace Xsd
{
    Checker::Checker(Xml::Document * xsdDoc):
        mXsdDoc(xsdDoc),
        mTypes(),
        mElementsTypes(),
        mAttributes(),
        mAttributesTypes()
    {
        //Building intermediary structure from xmlDoc
        //Initialize mRootType
        //TODO: Algo construction
    }

    Checker::~Checker()
    {
        delete mXsdDoc;

        for(typesMap::iterator iterator = mTypes.begin(); iterator != mTypes.end(); iterator++)
        {
            delete iterator->second;
        }
        mTypes.clear();

        mElementsTypes.clear();

        for(attributesMap::iterator iterator = mAttributes.begin(); iterator != mAttributes.end(); iterator++)
        {
            delete iterator->second;
        }
        mAttributes.clear();

        mAttributesTypes.clear();
    }

    bool
    Checker::isValid(Xml::Document * xmlDoc)
    {
        //TODO: Algo validation

        std::cerr << __func__ << " : not implemented yet" << std::endl;
        __builtin_trap();
    }
}
