/**
 * Provides the SplashState in the AGE namespace which is typically
 * the first state to be loaded for an application.
 *
 * @file src/AGE/Core/states/SplashState.hpp
 * @author Ryan Lindeman
 * @date 20100710 - Initial Release
 * @date 20110125 - Use the new RemoveActiveState not DropActiveState.
 * @date 20110127 - Moved to AGE Core library and src directory
 * @date 20110218 - Added ReInit method
 * @date 20110218 - Change to system include style
 * @date 20110625 - Added UpdateVariable and changed Update to UpdateFixed
 * @date 20110627 - Removed extra ; from namespace
 * @date 20110721 - Remove * from getAsset() calls since it now returns TYPE&
 * @date 20110906 - Change mApp from a pointer to an address reference
 * @date 20120322 - Support new SFML2 snapshot changes
 * @date 20120512 - Renamed App to Game since it really is just an interface
 * @date 20120702 - Changed Cleanup to HandleCleanup
 */
#include <AGE/Core/interfaces/Game.hpp>
#include <AGE/Core/states/SplashState.hpp>

namespace AGE
{
  SplashState::SplashState(Game& theApp, typeAssetID theSplashID,
      const std::string theFilename, float theDelay) :
    IState(ID32_("Splash"), theApp),
    mSplashID(theSplashID),
    mSplashFilename(theFilename),
    mSplashDelay(theDelay),
    mSplashImage(theFilename, AssetLoadNow, AssetLoadFromFile, AssetDropAtZero),
    mSplashSprite(mSplashImage.getAsset())
  {
  }

  SplashState::~SplashState(void)
  {
  }

  void SplashState::handleEvents(sf::Event theEvent)
  {
  }

  void SplashState::doInit(void)
  {
    // First call our base class implementation
    IState::doInit();
  }

  void SplashState::reInit(void)
  {
    // Do nothing yet
  }

  void SplashState::updateFixed(void)
  {
    // Drop our state after 10 seconds have elapsed
    if(false == isPaused() && getElapsedTime() > mSplashDelay)
    {
      mApp.mStateManager.removeActiveState();
    }
  }

  void SplashState::updateVariable(float theElapsedTime)
  {
  }

  void SplashState::draw(void)
  {

    // Draw our Splash sprite
    mApp.mWindow.draw(mSplashSprite);

  }

  void SplashState::handleCleanup(void)
  {
    // Do nothing
  }

} // namespace AGE

/**
 * Copyright (c) 2010-2011 Ryan Lindeman
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
