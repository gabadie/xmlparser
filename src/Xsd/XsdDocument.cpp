#include "XsdDocument.hpp"

#ifdef APP_DEBUG
#include <cassert>
#endif

namespace Xsd
{
    Document::Document(std::string xsdFilePath):
    mFilePath(xsdFilePath),
    mTypes(),
    mElementsTypes(),
    mAttributes(),
    mAttributesTypes()
    {

    }

    Document::~Document()
    {
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


}