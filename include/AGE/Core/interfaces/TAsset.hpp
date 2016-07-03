/**
 * Provides the TAsset template class for managing a single Asset and is used
 * by the IAssetHandler in the AGE namespace which is responsible for providing
 * the Asset management facilities for each asset type.
 *
 * @file include/AGE/Core/interfaces/TAsset.hpp
 * @author Ryan Lindeman
 * @date 20100723 - Initial Release
 * @date 20110127 - Moved to AGE Core library and include directory
 * @date 20110127 - Use new OS independent Uint/Int types
 * @date 20110131 - Added class and method argument documentation
 * @date 20110218 - Change to system include style
 * @date 20110627 - Removed extra ; from namespace
 * @date 20110721 - Make GetAsset method return TYPE& because that is usually
 *                  what SFML is expecting.
 * @date 20120503 - Rewrote TAsset to use RAII technique and more flexible
 *                  handling of Assets using the new IAssetHandler classes.
 * @date 20120514 - Fix comment whitespace and added GetID method call
 * @date 20120523 - Remove AGE_API from template classes to fix linker issues
 * @date 20120616 - Add default constructor and fixed assignment operator issues
 */
#ifndef   CORE_TASSET_HPP_INCLUDED
#define   CORE_TASSET_HPP_INCLUDED

#include <string>
#include <AGE/Core/interfaces/Game.hpp>
#include <AGE/Core/interfaces/TAssetHandler.hpp>
#include <AGE/Core/Core_types.hpp>

namespace AGE
{
  /// Provides base template class for all game assets
  template<class TYPE>
  class TAsset
  {
    public:
      /**
       * TAsset constructor that should be used most frequently since it
       * allows theAssetManager to provide the asset at construction time.
       * @param[in] theAssetID to use for this asset
       * @param[in] theLoadTime (Now, Later) of when to load this asset
       * @param[in] theLoadStyle (File, Mem, Network) to use when loading this asset
       * @param[in] theDropTime at (Zero, Exit) for when to unload this asset
       */
      TAsset(const assetID theAssetID,
        AssetLoadTime theLoadTime = AssetLoadLater,
        AssetLoadStyle theLoadStyle = AssetLoadFromFile,
        AssetDropTime theDropTime = AssetDropAtZero) :
        mAssetHandler(Game::getApp()->mAssetManager.getHandler<TYPE>()),
        mAsset(mAssetHandler.getReference(theAssetID, theLoadTime, theLoadStyle, theDropTime)),
        mAssetID(theAssetID)
      {
      }

      /**
       * TAsset default constructor that can be used if there is no way that
       * theAssetID can be provided at construction time. You will need to call
       * the SetID method before calling the GetAsset method if you use this
       * constructor.
       */
      TAsset() :
        mAssetHandler(Game::getApp()->mAssetManager.getHandler<TYPE>()),
        mAsset(mAssetHandler.getReference())
      {
      }

      /**
        * TAsset copy constructor will allow for copying of assets by
        * incrementing the reference counter for this asset through the
        * Asset Handler provided at construction time.
        */
      TAsset(const TAsset<TYPE>& theCopy) :
        mAssetHandler(theCopy.mAssetHandler),
        mAssetID(theCopy.mAssetID)
      {
        // Increment reference count to this asset
        mAsset = mAssetHandler.getReference(mAssetID);
      }


      /**
       * TAsset deconstructor
       */
      virtual ~TAsset()
      {
        // Drop reference to this asset
        mAssetHandler.dropReference(mAssetID);
      }

      /**
       * IsLoaded will return true if the Asset has been loaded.
       * @return true if loaded, false otherwise
       */
      bool isLoaded(void) const
      {
        return mAssetHandler.isLoaded(mAssetID);
      }

      /**
       * GetID will return the ID being used for this asset.
       * @return the Asset ID assigned to this asset
       */
      const assetID getID(void) const
      {
        return mAssetID;
      }

      /**
       * SetID will set the ID for this asset and get a reference to the
       * asset.
       * @param[in] theAssetID to use for this asset
       * @param[in] theLoadTime (Now, Later) of when to load this asset
       * @param[in] theLoadStyle (File, Mem, Network) to use when loading this asset
       * @param[in] theDropTime at (Zero, Exit) for when to unload this asset
       */
      void setID(const assetID theAssetID,
        AssetLoadTime theLoadTime = AssetLoadLater,
        AssetLoadStyle theLoadStyle = AssetLoadFromFile,
        AssetDropTime theDropTime = AssetDropAtZero)
      {
        // Make note of the new Asset ID
        mAssetID = theAssetID;

        // Try to obtain a reference to the new Asset from Handler
        mAsset = mAssetHandler.getReference(mAssetID, theLoadTime, theLoadStyle, theDropTime);
      }

      /**
       * GetAsset will return the Asset if it is available.
       * @return pointer to the Asset or NULL if not available yet.
       */
      TYPE& getAsset(void)
      {
        // Is asset not yet loaded, then try to load it immediately
        if(false == mAssetHandler.isLoaded(mAssetID))
        {
          // Load the asset immediately
          bool anLoaded = mAssetHandler.loadAsset(mAssetID);

          // If the asset was loaded, get a reference to it now
          if(anLoaded)
          {
            // Get reference to the asset immediately
            mAsset = mAssetHandler.getReference(mAssetID);
          }
        }

        // Return reference to dummy asset or loaded asset
        return *mAsset;
      }

      /**
       * GetFilename is responsible for retrieving the filename to use when
       * loading this asset.
       * @return the filename to use when loading this asset
       */
      const std::string getFilename(void)
      {
        return mAssetHandler.getFilename(mAssetID);
      }

      /**
       * SetFilename is responsible for setting the filename to use when
       * loading this asset.
       * @param[in] theFilename to use for loading asset
       */
      void setFilename(std::string theFilename)
      {
        // Set the filename to use for this asset
        mAssetHandler.setFilename(mAssetID, theFilename);
      }

      /**
       * GetLoadStyle will attempt to retrieve the loading style to use when
       * loading this asset. If the asset ID isn't found then
       * AssetLoadFromUnknown will be returned.
       * @return the loading style to use or AssetLoadFromUnknown otherwise
       */
      AssetLoadStyle getLoadStyle(void) const
      {
        return mAssetHandler.getLoadStyle(mAssetID);
      }

      /**
       * SetLoadStyle will set the loading style for this asset to theLoadStyle
       * specified.
       * @param[in] theLoadStyle (File, Mem, Network) to use when loading this asset
       */
      void setLoadTime(AssetLoadStyle theLoadStyle)
      {
        mAssetHandler.setLoadStyle(mAssetID, theLoadStyle);
      }

      /**
       * GetLoadTime will attempt to retrieve the loading time of when this
       * asset will be loaded.  If the asset ID isn't found then AssetLoadLater
       * will be returned.
       * @return the loading time or AssetLoadLater otherwise
       */
      AssetLoadTime getLoadTime(void) const
      {
        return mAssetHandler.getLoadTime(mAssetID);
      }

      /**
       * SetLoadTime will set the load time for this asset to theLoadTime
       * specified.
       * @param[in] theLoadTime (Now, Later) of when to load this asset
       */
      void setLoadTime(AssetLoadTime theLoadTime)
      {
        mAssetHandler.setLoadTime(mAssetID, theLoadTime);
      }

      /**
       * GetDropTime will attempt to retrieve the drop time of when this asset
       * will be unloaded.  If the asset ID isn't found then
       * AssetDropUnspecified will be returned.
       * @return the loading time or AssetDropUnspecified otherwise
       */
      AssetDropTime getDropTime(void) const
      {
        return mAssetHandler.getDropTime(mAssetID);
      }

      /**
       * SetDropTime will set the drop time for this asset to theDropTime
       * specified.
       * @param[in] theDropTime at (Zero, Exit) for when to unload this asset
       */
      void setDropTime(AssetDropTime theDropTime)
      {
        mAssetHandler.setDropTime(mAssetID, theDropTime);
      }

      /**
       * TAsset assignment operator will allow for copying of assets by
       * incrementing the reference count for this asset.
       * @param[in] theRight hand side of the = operation
       */
      TAsset<TYPE>& operator=(TAsset<TYPE> theRight)
      {
        // Now swap my local copy with theRight copy made during the call to this method
        swap(*this, theRight);

        // Increment the reference count to this Asset
        mAssetHandler.getReference(mAssetID);

        // Return my pointer
        return *this;
      }

      /**
       * swap was created according to the copy-and-swap idiom necessary for
       * correctly handling the assignment operator and copy constructor for a
       * resource holding class like TAsset. This allows us to perform
       * reference counting correctly for our TAsset resources.
       */
      friend void swap(TAsset& first, TAsset& second)
      {
        // enable ADL
        using std::swap;

        // Swap our asset pointer and ID
        swap(first.mAsset, second.mAsset);
        swap(first.mAssetID, second.mAssetID);
        // The mAssetHandler is already handled at construction time
      }
    protected:
      // Variables
      ///////////////////////////////////////////////////////////////////////////
      /// Asset Handler class that will manage this asset
      TAssetHandler<TYPE>& mAssetHandler;
      /// Pointer to the loaded asset
      TYPE*                mAsset;
      /// Asset ID specified for this asset
      assetID          mAssetID;
  }; // class TAsset
} // namespace AGE

#endif // CORE_TASSET_HPP_INCLUDED

/**
 * @class AGE::TAsset
 * @ingroup Core
 * The TAsset template class is the template used to create a new asset type
 * reference.  It provides indirect reference counting and dummy asset
 * references if no Asset ID is provided (see IAssetHandler).
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
