/**
 * Provides the MusicHandler class used by the AssetManager to manage all
 * sf::Music assets for the application.
 *
 * @file src/AGE/Core/assets/MusicHandler.cpp
 * @author Ryan Lindeman
 * @date 20120428 - Initial Release
 */
 
#include <AGE/Core/assets/MusicHandler.hpp>
#include <AGE/Core/loggers/Log_macros.hpp>
 
namespace AGE
{
  MusicHandler::MusicHandler() :
    TAssetHandler<sf::Music>()
  {
    ILOG() << "MusicHandler::ctor()" << std::endl;
  }
 
  MusicHandler::~MusicHandler()
  {
    ILOG() << "MusicHandler::dtor()" << std::endl;
  }

  bool MusicHandler::loadFromFile(const assetID theAssetID, sf::Music& theAsset)
  {
    // Start with a return result of false
    bool anResult = false;

    // Retrieve the filename for this asset
    std::string anFilename = getFilename(theAssetID);

    // Was a valid filename found? then attempt to load the asset from anFilename
    if(anFilename.length() > 0)
    {
      // Load the asset from a file

      anResult = theAsset.openFromFile(anFilename);

    }
    else
    {
      ELOG() << "MusicHandler::loadFromFile(" << theAssetID
        << ") No filename provided!" << std::endl;
    }

    // Return anResult of true if successful, false otherwise
    return anResult;
  }

  bool MusicHandler::loadFromMemory(const assetID theAssetID, sf::Music& theAsset)
  {
    // Start with a return result of false
    bool anResult = false;

    // TODO: Retrieve the const char* pointer to load data from
    const char* anData = NULL;
    // TODO: Retrieve the size in bytes of the font to load from memory
    size_t anDataSize = 0;

    // Try to obtain the font from the memory location specified
    if(NULL != anData && anDataSize > 0)
    {
      // Load the image from the memory location specified

      anResult = theAsset.openFromMemory(anData, anDataSize);

    }
    else
    {
      ELOG() << "MusicHandler::loadFromMemory(" << theAssetID
        << ") Bad memory location or size!" << std::endl;
    }

    // Return anResult of true if successful, false otherwise
    return anResult;
  }

  bool MusicHandler::loadFromNetwork(const assetID theAssetID, sf::Music& theAsset)
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
