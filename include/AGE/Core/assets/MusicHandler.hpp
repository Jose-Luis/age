/**
 * Provides the MusicHandler class used by the AssetManager to manage all
 * sf::Music assets for the application.
 *
 * @file include/AGE/Core/assets/MusicHandler.hpp
 * @author Ryan Lindeman
 * @date 20120428 - Initial Release
 */
#ifndef   CORE_MUSIC_HANDLER_HPP_INCLUDED
#define   CORE_MUSIC_HANDLER_HPP_INCLUDED
 
#include <SFML/Audio.hpp>
#include <AGE/Core/Core_types.hpp>
#include <AGE/Core/interfaces/TAssetHandler.hpp>

namespace AGE
{
  /// Provides the MusicHandler class for managing sf::Music assets
  class AGE_API MusicHandler : public TAssetHandler<sf::Music>
  {
  public:
    /**
     * MusicHandler constructor
     */
    MusicHandler();
 
    /**
     * MusicHandler deconstructor
     */
    virtual ~MusicHandler();
 
  protected:
    /**
     * LoadFromFile is responsible for loading theAsset from a file and must
     * be defined by the derived class since the interface for TYPE is
     * unknown at this stage.
     * @param[in] theAssetID of the asset to be loaded
     * @param[in] theAsset pointer to load
     * @return true if the asset was successfully loaded, false otherwise
     */
    virtual bool loadFromFile(const assetID theAssetID, sf::Music& theAsset);

    /**
     * LoadFromMemory is responsible for loading theAsset from memory and
     * must be defined by the derived class since the interface for TYPE is
     * unknown at this stage.
     * @param[in] theAssetID of the asset to be loaded
     * @param[in] theAsset pointer to load
     * @return true if the asset was successfully loaded, false otherwise
     */
    virtual bool loadFromMemory(const assetID theAssetID, sf::Music& theAsset);

    /**
     * LoadFromNetwork is responsible for loading theAsset from network and
     * must be defined by the derived class since the interface for TYPE is
     * unknown at this stage.
     * @param[in] theAssetID of the asset to be loaded
     * @param[in] theAsset pointer to load
     * @return true if the asset was successfully loaded, false otherwise
     */
    virtual bool loadFromNetwork(const assetID theAssetID, sf::Music& theAsset);

  private:
  }; // class MusicHandler
} // namespace AGE

#endif // CORE_MUSIC_MANAGER_HPP_INCLUDED

/**
 * @class AGE::MusicHandler
 * @ingroup Core
 * The MusicHandler class is used to reference count and manage all sf::Image
 * classes used in a AGE application.
 *
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
