/**
 * Provides the StateManager class in the AGE namespace which is responsible
 * for providing the State management facilities to the App base class used in
 * the AGE core library.
 *
 * @file src/AGE/Core/classes/StateManager.cpp
 * @author Ryan Lindeman
 * @date 20100728 - Initial Release
 * @date 20110120 - Added ability to add inactive states and initialize them
 *                  when they become active and send Dialog signal events.
 * @date 20110120 - Add ability to drop active state as inactive state
 * @date 20110125 - IState::HandleCleanup is now called from here
 * @date 20110127 - Moved to AGE Core library and src directory
 * @date 20110218 - Change mDropped to mDead to remove potential confusion
 * @date 20110218 - Added InactivateActiveState and ResetActiveState methods
 * @date 20110218 - Change to system include style
 * @date 20110611 - Convert logging to new Log macros
 * @date 20110627 - Removed extra ; from namespace
 * @date 20110810 - Return address not pointer from GetActiveState.
 * @date 20120426 - Add another sanity check in HandleCleanup for active state
 * @date 20120512 - Renamed App to Game since it really is just an interface
 * @date 20120702 - Rename HandleCleanup to Cleanup.
 */

#include <assert.h>
#include <stddef.h>
#include <AGE/Core/loggers/Log_macros.hpp>
#include <AGE/Core/classes/StateManager.hpp>
#include <AGE/Core/interfaces/Game.hpp>
#include <AGE/Core/interfaces/IState.hpp>

namespace AGE
{
  StateManager::StateManager() :
    mApp(NULL)
  {
    ILOGM("StateManager::ctor()");
  }

  StateManager::~StateManager()
  {
    ILOGM("StateManager::dtor()");

    // Drop all active states
    while(!mStack.empty())
    {
      // Retrieve the currently active state
      IState* anState = mStack.back();
 
      // Pop the currently active state off the stack
      mStack.pop_back();

      // Pause the currently active state
      anState->pause();

      // De-initialize the state
      anState->deInit();

      // Handle the cleanup before we pop it off the stack
      anState->cleanup();

      // Just delete the state now
      delete anState;

      // Don't keep pointers around we don't need
      anState = NULL;
    }

    // Delete all our dropped states
    while(!mDead.empty())
    {
      // Retrieve the currently active state
      IState* anState = mDead.back();

      // Pop the currently active state off the stack
      mDead.pop_back();

      // Pause the currently active state
      anState->pause();

      // De-initialize the state
      anState->deInit();

      // Handle the cleanup before we pop it off the stack
      anState->cleanup();

      // Just delete the state now
      delete anState;

      // Don't keep pointers around we don't need
      anState = NULL;
    }

    // Clear pointers we don't need anymore
    mApp = NULL;
  }

  void StateManager::registerApp(Game* theApp)
  {
    // Check that our pointer is good
    assert(NULL != theApp && "StateManager::registerApp() theApp pointer provided is bad");

    // Make a note of the pointer
    assert(NULL == mApp && "StateManager::registerApp() theApp pointer was already registered");
    mApp = theApp;
  }

  bool StateManager::isEmpty(void)
  {
    return mStack.empty();
  }

  void StateManager::addActiveState(IState* theState)
  {
    // Check that they didn't provide a bad pointer
    assert(NULL != theState && "StateManager::addActiveState() received a bad pointer");

    // Log the adding of each state
    ILOG() << "StateManager::addActiveState(" << theState->getID() << ")" << std::endl;

    // Is there a state currently running? then Pause it
    if(!mStack.empty())
    {
      // Pause the currently running state since we are changing the
      // currently active state to the one provided
      mStack.back()->pause();
    }

    // Add the active state
    mStack.push_back(theState);

    // Initialize the new active state
    mStack.back()->doInit();
  }

  void StateManager::addInactiveState(IState* theState)
  {
    // Check that they didn't provide a bad pointer
    assert(NULL != theState && "StateManager::addInactiveState() received a bad pointer");

    // Log the adding of each state
    ILOG() << "StateManager::addInactiveState(" << theState->getID() << ")" << std::endl;

    // Add the inactive state to the bottom of the stack
    mStack.insert(mStack.begin(), theState);
  }

  IState& StateManager::getActiveState(void)
  {
    return *mStack.back();
  }

  void StateManager::inactivateActivateState(void)
  {
    // Is there no currently active state to drop?
    if(!mStack.empty())
    {
      // Retrieve the currently active state
      IState* anState = mStack.back();

      // Log the inactivating an active state
      ILOG() << "StateManager::inactivateActiveState(" << anState->getID() << ")" << std::endl;

      // Pause the currently active state
      anState->pause();

      // Pop the currently active state off the stack
      mStack.pop_back();

      // Move this now inactive state to the absolute back of our stack
      mStack.insert(mStack.begin(), anState);

      // Don't keep pointers around we don't need anymore
      anState = NULL;
    }
    else
    {
      // Quit the application with an error status response
      if(NULL != mApp)
      {
        mApp->quit(StatusAppStackEmpty);
      }
      return;
    }

    // Is there another state to activate? then call Resume to activate it
    if(!mStack.empty())
    {
      // Has this state ever been initialized?
      if(mStack.back()->isInitComplete())
      {
        // Resume the new active state
        mStack.back()->resume();
      }
      else
      {
        // Initialize the new active state
        mStack.back()->doInit();
      }
    }
    else
    {
      // There are no states on the stack, exit the program
      if(NULL != mApp)
      {
        mApp->quit(StatusAppOK);
      }
    }
  }

  void StateManager::dropActiveState(void)
  {
    // Is there no currently active state to drop?
    if(!mStack.empty())
    {
      // Retrieve the currently active state
      IState* anState = mStack.back();

      // Log the dropping of an active state
      ILOG() << "StateManager::dropActiveState(" << anState->getID() << ")" << std::endl;

      // Pause the currently active state
      anState->pause();

      // Deinit currently active state before we pop it off the stack
      // (handleCleanup() will be called by IState::doInit() method if this
      // state is ever set active again)
      anState->deInit();

      // Pop the currently active state off the stack
      mStack.pop_back();

      // Move this now inactive state to the absolute back of our stack
      mStack.insert(mStack.begin(), anState);

      // Don't keep pointers around we don't need anymore
      anState = NULL;
    }
    else
    {
      // Quit the application with an error status response
      if(NULL != mApp)
      {
        mApp->quit(StatusAppStackEmpty);
      }
      return;
    }

    // Is there another state to activate? then call Resume to activate it
    if(!mStack.empty())
    {
      // Has this state ever been initialized?
      if(mStack.back()->isInitComplete())
      {
        // Resume the new active state
        mStack.back()->resume();
      }
      else
      {
        // Initialize the new active state
        mStack.back()->doInit();
      }
    }
    else
    {
      // There are no states on the stack, exit the program
      if(NULL != mApp)
      {
        mApp->quit(StatusAppOK);
      }
    }
  }

  void StateManager::resetActiveState(void)
  {
    // Is there no currently active state to reset?
    if(!mStack.empty())
    {
      // Retrieve the currently active state
      IState* anState = mStack.back();

      // Log the resetting of an active state
      ILOG() << "StateManager::resetActiveState(" << anState->getID() << ")" << std::endl;

      // Pause the currently active state
      anState->pause();

      // Call the ReInit method to Reset the currently active state
      anState->reInit();

      // Resume the currently active state
      anState->resume();

      // Don't keep pointers around we don't need anymore
      anState = NULL;
    }
    else
    {
      // Quit the application with an error status response
      if(NULL != mApp)
      {
        mApp->quit(StatusAppStackEmpty);
      }
      return;
    }
  }

  void StateManager::removeActiveState(void)
  {
    // Is there no currently active state to drop?
    if(!mStack.empty())
    {
      // Retrieve the currently active state
      IState* anState = mStack.back();

      // Log the removing of an active state
      ILOG() << "StateManager::removeActiveState(" << anState->getID() << ")" << std::endl;

      // Pause the currently active state
      anState->pause();
 
      // Deinitialize the currently active state before we pop it off the stack
      anState->deInit();

      // Pop the currently active state off the stack
      mStack.pop_back();

      // Move this state to our dropped stack
      mDead.push_back(anState);

      // Don't keep pointers around we don't need anymore
      anState = NULL;
    }
    else
    {
      // Quit the application with an error status response
      if(NULL != mApp)
      {
        mApp->quit(StatusAppStackEmpty);
      }
      return;
    }

    // Is there another state to activate? then call Resume to activate it
    if(!mStack.empty())
    {
      // Has this state ever been initialized?
      if(mStack.back()->isInitComplete())
      {
        // Resume the new active state
        mStack.back()->resume();
      }
      else
      {
        // Initialize the new active state
        mStack.back()->doInit();
      }
    }
    else
    {
      // There are no states on the stack, exit the program
      if(NULL != mApp)
      {
        mApp->quit(StatusAppOK);
      }
    }
  }

  void StateManager::setActiveState(Id theStateID)
  {
    std::vector<IState*>::iterator it;

    // Find the state that matches theStateID
    for(it=mStack.begin(); it < mStack.end(); it++)
    {
      // Does this state match theStateID? then activate it as the new
      // currently active state
      if((*it)->getID() == theStateID)
      {
        // Get a pointer to soon to be currently active state
        IState* anState = *it;

        // Log the setting of a previously active state as the current active state
        ILOG() << "StateManager::setActiveState(" << anState->getID() << ")" << std::endl;

        // Erase it from the list of previously active states
        mStack.erase(it);

        // Is there a state currently running? then Pause it
        if(!mStack.empty())
        {
          // Pause the currently running state since we are changing the
          // currently active state to the one specified by theStateID
          mStack.back()->pause();
        }

        // Add the new active state
        mStack.push_back(anState);

        // Don't keep pointers we don't need around
        anState = NULL;

        // Has this state ever been initialized?
        if(mStack.back()->isInitComplete())
        {
          // Resume the new active state
          mStack.back()->resume();
        }
        else
        {
          // Initialize the new active state
          mStack.back()->doInit();
        }

        // Exit our find loop
        break;
      } // if((*it)->getID() == theStateID)
    } // for(it=mStack.begin(); it < mStack.end(); it++)
  }

  void StateManager::cleanup(void)
  {
    // Always call our cleanup events with our pointer when this method is called
    mCleanupEvents.doEvents();

    // Remove one of our dead states
    if(!mDead.empty())
    {
      // Retrieve the dead state
      IState* anState = mDead.back();
      assert(NULL != anState && "StateManager::handleCleanup() invalid dropped state pointer");

      // Pop the dead state off the stack
      mDead.pop_back();

      // Call the DeInit if it hasn't been called yet
      if(anState->isInitComplete())
      {
        anState->deInit();
      }

      // Handle the cleanup before we delete anState
      anState->cleanup();

      // Just delete the state now
      delete anState;

      // Don't keep pointers around we don't need
      anState = NULL;
    }

    // Make sure we still have an active state
    if(NULL == mStack.back())
    {
      // There are no states on the stack, exit the program
      if(NULL != mApp)
      {
        mApp->quit(StatusAppOK);
      }
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
