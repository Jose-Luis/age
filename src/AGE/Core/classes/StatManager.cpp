/**
 * Provides the StatManager class in the AGE namespace which is responsible
 * for collecting and providing Statistical information about the application.
 * This information includes, the current Updates per second and Frames per
 * second and other statistics.
 *
 * @file src/AGE/Core/classes/StatManager.hpp
 * @author Ryan Lindeman
 * @date 20110128 - Initial Release
 * @date 20110128 - Moved to AGE Core library and src directory
 * @date 20110218 - Change to system include style
 * @date 20110611 - Convert logging to new Log macros
 * @date 20110625 - Added UpdateVariable and changed Update to UpdateFixed
 * @date 20110627 - Fixed ctor init order and removed extra ; from namespace
 * @date 20110704 - Changed Move to SetPosition
 * @date 20120211 - Support new SFML2 snapshot changes
 * @date 20120322 - Support new SFML2 snapshot changes
 * @date 20120421 - Use arial.ttf font since SFML 2 crashes on exit when using default font
 * @date 20120512 - Renamed App to Game since it really is just an interface
 * @date 20120518 - Use sf::Font instead of FontAsset to remove circular dependency
 * @date 20120609 - Whitespace changes
 * @date 20120616 - Add std::nothrow to new commands for mFPS and mUPS
 */

#include <assert.h>
#include <sstream>
#include <AGE/Core/loggers/Log_macros.hpp>
#include <AGE/Core/classes/StatManager.hpp>
#include <AGE/Core/interfaces/Game.hpp>

namespace AGE
{
  StatManager::StatManager() :
    mApp(NULL),
    mShow(false),
    mFrames(0),
    mFrameClock(),
    mDefaultFont(),
    mFPS(NULL),
    mUpdates(0),
    mUpdateClock(),
    mUPS(NULL)
  {
    ILOGM("StatManager::ctor()");
		mDefaultFont.loadFromFile("resources/arial.ttf");
  }

  StatManager::~StatManager()
  {
    ILOGM("StatManager::dtor()");

    // Clear pointers we don't need anymore
    mApp = NULL;
  }

  void StatManager::doInit(void)
  {
    ILOGM("StatManager::DoInit()");

    // Reset our counters
    mFrames = 0;
    mUpdates = 0;

    // Reset our clocks

    mFrameClock.restart();
    mUpdateClock.restart();

    // Position and color for the FPS/UPS string
    mFPS = new(std::nothrow) sf::Text("", mDefaultFont, 30);
    mFPS->setFillColor(sf::Color(0,255,0,128));
    mFPS->setPosition(0,0);

    mUPS = new(std::nothrow) sf::Text("", mDefaultFont, 30);
    mUPS->setFillColor(sf::Color(0,255,0,128));
    mUPS->setPosition(0,30);

  }

  void StatManager::deInit(void)
  {
    ILOGM("StatManager::DeInit()");

    // Delete our FPS string
    delete mFPS;
    mFPS = NULL;

    // Delete our UPS string
    delete mUPS;
    mUPS = NULL;
  }

  bool StatManager::isShowing(void) const
  {
    return mShow;
  }

  void StatManager::setShow(bool theShow)
  {
    mShow = theShow;
  }

  Uint32 StatManager::getUpdates(void) const
  {
    return mUpdates;
  }

  Uint32 StatManager::getFrames(void) const
  {
    return mFrames;
  }

  void StatManager::registerApp(Game* theApp)
  {
    // Check that our pointer is good
    assert(NULL != theApp && "StatManager::registerApp() theApp pointer provided is bad");

    // Make a note of the pointer
    assert(NULL == mApp && "StatManager::registerApp() theApp pointer was already registered");
    mApp = theApp;
  }

  void StatManager::updateFixed(void)
  {
    // Check our App pointer
    assert(NULL != mApp && "StatManager::update() bad app pointer");

    // Increment our update counter
    mUpdates++;

      if(mUpdateClock.getElapsedTime().asSeconds() > 1.0f)

      {
        // Updates string stream
        std::ostringstream updates;

        // Update our UPS string to be displayed
        updates.precision(2);
        updates.width(7);
        updates << "UPS: " << std::fixed << mUpdates;

        mUPS->setString(updates.str());


        // Reset our Update clock and update counter
        mUpdates = 0;

        mUpdateClock.restart();

      }
  }

  void StatManager::draw(void)
  {
    // Check our mApp pointer
    assert(NULL != mApp && "StatManager::draw() invalid app pointer provided");

    // Increment our frame counter
    mFrames++;

    if(mFrameClock.getElapsedTime().asSeconds() > 1.0f)

    {
      // Frames string stream
      std::ostringstream frames;

      // Get our FramesPerSecond value
      frames.precision(2);
      frames.width(7);
      frames << "FPS: " << std::fixed << mFrames;

      mFPS->setString(frames.str());


      // Reset our Frames clock and frame counter
      mFrames = 0;

      mFrameClock.restart();

    }

    // Are we showing the current statistics?
    if(mShow)
    {

      // Draw the Frames Per Second debug value on the screen
      mApp->mWindow.draw(*mFPS);

      // Draw the Updates Per Second debug value on the screen
      mApp->mWindow.draw(*mUPS);

    }
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
