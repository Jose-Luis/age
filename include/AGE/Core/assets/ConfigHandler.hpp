/**
 * Provides the ConfigHandler class used by the AssetManager to manage all
 * ConfigReader assets for the application.
 *
 * @file include/AGE/Core/assets/ConfigHandler.hpp
 * @author Ryan Lindeman
 * @date 20120428 - Initial Release
 */
#ifndef   CORE_CONFIG_HANDLER_HPP_INCLUDED
#define   CORE_CONFIG_HANDLER_HPP_INCLUDED
 
#include <AGE/Core/classes/ConfigReader.hpp>
#include <AGE/Core/interfaces/TAssetHandler.hpp>
#include <AGE/Core/Core_types.hpp>

namespace AGE
{
  /// Provides the ConfigHandler class for managing ConfigReader assets
  class AGE_API ConfigHandler : public TAssetHandler<ConfigReader>
  {
  public:
    /**
     * ConfigHandler constructor
     */
    ConfigHandler();
 
    /**
     * ConfigHandler deconstructor
     */
    virtual ~ConfigHandler();
 
  protected:
    /**
     * LoadFromFile is responsible for loading theAsset from a file and must
     * be defined by the derived class since the interface for TYPE is
     * unknown at this stage.
     * @param[in] theAssetID of the asset to be loaded
     * @param[in] theAsset pointer to load
     * @return true if the asset was successfully loaded, false otherwise
     */
    virtual bool loadFromFile(const assetID theAssetID, ConfigReader& theAsset);

    /**
     * LoadFromMemory is responsible for loading theAsset from memory and
     * must be defined by the derived class since the interface for TYPE is
     * unknown at this stage.
     * @param[in] theAssetID of the asset to be loaded
     * @param[in] theAsset pointer to load
     * @return true if the asset was successfully loaded, false otherwise
     */
    virtual bool loadFromMemory(const assetID theAssetID, ConfigReader& theAsset);

    /**
     * LoadFromNetwork is responsible for loading theAsset from network and
     * must be defined by the derived class since the interface for TYPE is
     * unknown at this stage.
     * @param[in] theAssetID of the asset to be loaded
     * @param[in] theAsset pointer to load
     * @return true if the asset was successfully loaded, false otherwise
     */
    virtual bool loadFromNetwork(const assetID theAssetID, ConfigReader& theAsset);

  private:
  }; // class ConfigHandler
} // namespace AGE

#endif // CORE_CONFIG_MANAGER_HPP_INCLUDED

/**
 * @class AGE::ConfigHandler
 * @ingroup Core
 * The ConfigHandler class is used to reference count and manage all
 * ConfigReader classes used in a AGE application.
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
