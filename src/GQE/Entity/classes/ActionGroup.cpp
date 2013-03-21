/**
 * Provides a class that manages all active and inactive IAction classes for an
 * IEntity. This is typically stored as a property using the PropertyManager
 * class.
 *
 * @file include/GQE/Entity/classes/ActionGroup.cpp
 * @author Jacob Dix
 * @date 20120627 - Initial Release
 */

#include <GQE/Entity/classes/ActionGroup.hpp>
#include <GQE/Entity/systems/ActionSystem.hpp>
#include <GQE/Entity/interfaces/IAction.hpp>

namespace GQE
{
  ActionGroup::ActionGroup() :
    mActionSystem(NULL)
  {
  }

  ActionGroup::ActionGroup(ActionSystem* theActionSystem) :
    mActionSystem(theActionSystem)
  {
  }

  ActionGroup::~ActionGroup()
  {
    // Clear our Active and Disabled action lists
    mActive.clear();
    mDisabled.clear();
  }

  bool ActionGroup::isAvailable(const typeActionID theActionID) const
  {
    bool anResult = false;

    // See if theActionID was found in our list of active actions
    anResult |= (mActive.find(theActionID) != mActive.end());

    // See if theActionID was found in our list of disabled actions
    anResult |= (mDisabled.find(theActionID) != mDisabled.end());

    // Return true if theActionID was found above, false otherwise
    return anResult;
  }

  bool ActionGroup::isActive(const typeActionID theActionID) const
  {
    bool anResult = false;

    // See if theActionID was found in our list of active actions
    anResult = (mActive.find(theActionID) != mActive.end());

    // Return true if theActionID was found above, false otherwise
    return anResult;
  }

  void ActionGroup::activate(const typeActionID theActionID)
  {
    std::map<const typeActionID, IAction*>::iterator anIter;
    anIter = mDisabled.find(theActionID);

    // Make sure theActionID is currently disabled
    if(anIter != mDisabled.end())
    {
      // Keep track of IAction pointer from disabled list
      IAction* anAction = anIter->second;

      // Remove IAction pointer from disabled list
      mDisabled.erase(anIter);

      // Is this already in our active list? then don't add it
      if(mActive.find(theActionID) == mActive.end())
      {
        // Add IAction pointer to active list
        mActive.insert(std::pair<const typeActionID, IAction*>(theActionID, anAction));
      }
      else
      {
        ELOG() << "ActionGroup::activate(" << theActionID
          << ") is already in active list, removing from disabled list" << std::endl;
      }
    }
  }

  void ActionGroup::disable(const typeActionID theActionID)
  {
    std::map<const typeActionID, IAction*>::iterator anIter;
    anIter = mActive.find(theActionID);

    // Make sure theActionID is currently active
    if(anIter != mActive.end())
    {
      // Keep track of IAction pointer from active list
      IAction* anAction = anIter->second;

      // Remove IAction pointer from active list
      mActive.erase(anIter);

      // Is this already in our disabled list? then don't add it
      if(mDisabled.find(theActionID) == mDisabled.end())
      {
        // Add IAction pointer to disabled list
        mDisabled.insert(std::pair<const typeActionID, IAction*>(theActionID, anAction));
      }
      else
      {
        ELOG() << "ActionGroup::disable(" << theActionID
          << ") is already in disabled list, removing from active list" << std::endl;
      }
    }
  }

  void ActionGroup::add(const typeActionID theActionID)
  {
    // Make sure theActionID hasn't been previously registered with this ActionGroup
    if(isAvailable(theActionID) == false)
    {
      if(mActionSystem != NULL)
      {
        // Get anAction matching theActionID from the ActionSystem class
        IAction* anAction = mActionSystem->getAction(theActionID);

        // Was a valid IAction pointer returned? then add to our disabled list
        if(anAction != NULL)
        {
          // Add this IAction to our disabled list
          mDisabled.insert(std::pair<const typeActionID, IAction*>(theActionID, anAction));
        }
        else
        {
          ELOG() << "ActionGroup::add(" << theActionID
            << ") does not exist in ActionSystem class!" << std::endl;
        }
      }
      else
      {
        ELOG() << "ActionGroup::add(" << theActionID
          << ") missing ActionSystem pointer, please call SetActionSystem first!"
          << std::endl;
      }
    }
    else
    {
      WLOG() << "ActionGroup::add(" << theActionID
        << ") action is already registered!" << std::endl;
    }
  }

  void ActionGroup::drop(const typeActionID theActionID)
  {
    // Make sure theActionID is currently registered with this ActionGroup
    if(isAvailable(theActionID))
    {
      std::map<const typeActionID, IAction*>::iterator anIter;
      // See if theActionID is in our active list
      anIter = mActive.find(theActionID);
      if(anIter != mActive.end())
      {
        // Erase the action from the active list
        mActive.erase(anIter);
      }
      // See if theActionID is in our disabled list
      anIter = mDisabled.find(theActionID);
      if(anIter != mDisabled.end())
      {
        // Erase the action from the disabled list
        mDisabled.erase(anIter);
      }
    }
    else
    {
      WLOG() << "ActionGroup::drop(" << theActionID
        << ") action is not registered!" << std::endl;
    }
  }

  void ActionGroup::doActions(IEntity* theEntity)
  {
    // Make sure clear all of our IAction classes
    std::map<const typeActionID, IAction*>::iterator anActionIter;

    // Start at the beginning of the list of IAction classes
    anActionIter = mActive.begin();
    while(anActionIter != mActive.end())
    {
      // Get the IAction to execute
      IAction* anAction = anActionIter->second;

      // Move on to the next iterator
      anActionIter++;

      // Call the IAction DoAction method and provide theEntity to it
      anAction->doAction(theEntity);
    }
  }

  void ActionGroup::setActionSystem(ActionSystem* theActionSystem)
  {
    if(theActionSystem != NULL)
    {
      // Is this a different one than the last one?
      if(mActionSystem != NULL && mActionSystem != theActionSystem)
      {
        WLOG() << "ActionGroup::setActionSystem() replacing previous ActionSystem" << std::endl;
      }

      // Make note of the new ActionSystem address
      mActionSystem = theActionSystem;
    }
    else
    {
      ELOG() << "ActionGroup::setActionSystem() null ActionSystem provided!" << std::endl;
    }
  }
} // namespace GQE

/**
 * Copyright (c) 2010-2012 Jacob Dix
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
