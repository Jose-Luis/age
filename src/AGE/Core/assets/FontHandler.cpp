/**
 * Provides the FontHandler class used by the AssetManager to manage all
 * sf::Font assets for the application.
 *
 * @file src/AGE/Core/assets/FontHandler.cpp
 * @author Ryan Lindeman
 * @date 20120428 - Initial Release
 */
 
#include <AGE/Core/assets/FontHandler.hpp>
#include <AGE/Core/loggers/Log_macros.hpp>
 
namespace AGE
{
  FontHandler::FontHandler() :
    TAssetHandler<sf::Font>()
  {
    ILOG() << "FontHandler::ctor()" << std::endl;
  }
 
  FontHandler::~FontHandler()
  {
    ILOG() << "FontHandler::dtor()" << std::endl;
  }

  bool FontHandler::loadFromFile(const assetID theAssetID, sf::Font& theAsset)
  {
    // Start with a return result of false
    bool anResult = false;

    // Retrieve the filename for this asset
    std::string anFilename = getFilename(theAssetID);

    // Was a valid filename found? then attempt to load the asset from anFilename
    if(anFilename.length() > 0)
    {
      // Load the asset from a file

      anResult = theAsset.loadFromFile(anFilename);

    }
    else
    {
      ELOG() << "FontHandler::loadFromFile(" << theAssetID
        << ") No filename provided!" << std::endl;
    }

    // Return anResult of true if successful, false otherwise
    return anResult;
  }

  bool FontHandler::loadFromMemory(const assetID theAssetID, sf::Font& theAsset)
  {
    // Start with a return result of false
    bool anResult = false;

    // TODO: Retrieve the const char* pointer to load data from
    const char* anData = NULL;
    // TODO: Retrieve the size in bytes of the font to load from memory
    size_t anDataSize = 0;
    // TODO: Retrieve the character size for this font

    // UNUSED VARIABLE ->  unsigned int anCharSize = 30;

    // Try to obtain the font from the memory location specified
    if(NULL != anData && anDataSize > 0)
    {
      // Load the font from the memory location specified

      anResult = theAsset.loadFromMemory(anData, anDataSize);

    }
    else
    {
      ELOG() << "FontHandler::loadFromMemory(" << theAssetID
        << ") Bad memory location or size!" << std::endl;
    }

    // Return anResult of true if successful, false otherwise
    return anResult;
  }

  bool FontHandler::loadFromNetwork(const assetID theAssetID, sf::Font& theAsset)
  {
    // Start with a return result of false
    bool anResult = false;

    // TODO: Add load from network for this asset

    // Return anResult of true if successful, false otherwise
    return anResult;
  }
} // namespace AGE
 
/**
 * Copyright (c) 2010-2012 Ryan Lindeman
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
