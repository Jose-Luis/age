/**
 * Provides the IState class in the AGE namespace which is responsible for
 * providing the IState interface used in the AGE core library.
 *
 * @file src/AGE/Core/interfaces/IState.cpp
 * @author Ryan Lindeman
 * @date 20110801 - Initial Release
 * @date 20120211 - Support new SFML2 snapshot changes
 * @date 20120322 - Support new SFML2 snapshot changes
 * @date 20120512 - Renamed App to Game since it really is just an interface
 * @date 20120702 - Switched names of Cleanup and HandleCleanup and added cleanup events
 */

#include <assert.h>
#include <AGE/Core/interfaces/IState.hpp>
#include <AGE/Core/loggers/Log_macros.hpp>
#include <AGE/Core/interfaces/Game.hpp>

namespace AGE
{
  IState::IState(const typeStateID theStateID, Game& theApp) :
    mApp(theApp),
    mStateID(theStateID),
    mInit(false),
    mPaused(false),
    mCleanup(false),
    mElapsedTime(0.0f),
    mPausedTime(0.0f)
  {
    ILOG() << "IState::ctor(" << mStateID << ")" << std::endl;
  }

  IState::~IState()
  {
    ILOG() << "IState::dtor(" << mStateID << ")" << std::endl;
  }

  const typeStateID IState::getID(void) const
  {
    return mStateID;
  }

  void IState::doInit(void)
  {
    ILOG() << "IState::doInit(" << mStateID << ")" << std::endl;

    // If Cleanup hasn't been called yet, call it now!
    if(true == mCleanup)
    {
      handleCleanup();
    }

    // Initialize if necessary
    if(false == mInit)
    {
      mInit = true;
      mPaused = false;
      mElapsedTime = 0.0f;

      mElapsedClock.restart();

      mPausedTime = 0.0f;

      mPausedClock.restart();

    }
  }

  void IState::deInit(void)
  {
    ILOG() << "IState::deInit(" << mStateID << ")" << std::endl;

    if(true == mInit)
    {
      mCleanup = true;
      mInit = false;

      mElapsedTime += mElapsedClock.getElapsedTime().asSeconds();

      if(true == mPaused)
      {

        mPausedTime += mPausedClock.getElapsedTime().asSeconds();

      }
    }
  }

  bool IState::isInitComplete(void)
  {
    return mInit;
  }

  bool IState::isPaused(void)
  {
    return mPaused;
  }

  void IState::pause(void)
  {
    ILOG() << "IState::pause(" << mStateID << ")" << std::endl;

    if(false == mPaused)
    {
      mPaused = true;

      mPausedClock.restart();

    }
  }

  void IState::resume(void)
  {
    ILOG() << "IState::resume(" << mStateID << ")" << std::endl;

    if(true == mPaused)
    {
      mPaused = false;

      mPausedTime += mPausedClock.getElapsedTime().asSeconds();


    }
  }

  void IState::handleEvents(sf::Event theEvent)
  {
  // Exit program if Escape key is pressed
    if(

      (theEvent.type == sf::Event::KeyReleased) && (theEvent.key.code == sf::Keyboard::Escape)

      )
    {
      // Signal the application to exit
      mApp.quit(AGE::StatusAppOK);
    }
  }

  float IState::getElapsedTime(void) const
  {

    float result = mElapsedClock.getElapsedTime().asSeconds();


    if(false == mInit)
    {
      result = mElapsedTime;
    }

    return result;
  }

  void IState::cleanup(void)
  {
    // This will be true if this IState is about to be deleted soon
    if(true == mCleanup)
    {
      // Call our handle cleanup virtual method
      handleCleanup();

      // Clear our cleanup flag
      mCleanup = false;
    }
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
