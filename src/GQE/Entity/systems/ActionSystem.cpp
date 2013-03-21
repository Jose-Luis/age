/**
 * Provides the ActionSystem class for handing all IEntity actions in a game
 *
 * @file src/GQE/Entity/systems/ActionSystem.cpp
 * @author Jacob Dix
 * @date 20120620 - Initial Release
 */
#include <SFML/Graphics.hpp>
#include <GQE/Entity/systems/ActionSystem.hpp>
#include <GQE/Entity/interfaces/IAction.hpp>
#include <GQE/Entity/interfaces/IEntity.hpp>
#include <GQE/Entity/classes/ActionGroup.hpp>

namespace GQE
{
  ActionSystem::ActionSystem(IApp& theApp):
    ISystem("ActionSystem",theApp)
  {
  }

  ActionSystem::~ActionSystem()
  {
    // Make sure clear all of our IAction classes
    std::map<const typeActionID, IAction*>::iterator anActionIter;

    // Start at the beginning of the list of IAction classes
    anActionIter = mActions.begin();
    while(anActionIter != mActions.end())
    {
      // Erase this IAction and move to the next one
      eraseAction(anActionIter++);
    }

    // Last of all clear our list of IActions
    mActions.clear();
  }

  IAction* ActionSystem::getAction(const typeActionID theActionID)
  {
    IAction* anResult = NULL;
    std::map<const typeActionID, IAction*>::iterator iter;

    // Is theActionID found in our list of available actions?
    iter = mActions.find(theActionID);
    if(iter != mActions.end())
    {
      // Retrieve the IAction pointer found
      anResult = iter->second;
    }

    // Return the IAction found above or NULL if none was found
    return anResult;
  }

  void ActionSystem::addAction(IAction* theAction)
  {
    if(theAction != NULL)
    {
      // Make sure theAction doesn't already exist
      if(mActions.find(theAction->getID()) == mActions.end())
      {
        // Add theAction provided to our map
        mActions.insert(
            std::pair<const typeActionID, IAction*>(
              theAction->getID(), theAction));
      }
      else
      {
        WLOG() << "ActionSystem::addAction(" << theAction->getID()
          << ") action already exists!" << std::endl;
      }
    }
    else
    {
      ELOG() << "ActionSystem::addAction() Null pointer provided!" << std::endl;
    }
  }

  void ActionSystem::addProperties(IEntity* theEntity)
  {
    theEntity->mProperties.add<ActionGroup>("Actions",ActionGroup(this));
  }

  void ActionSystem::handleEvents(sf::Event theEvent)
  {
  }

  void ActionSystem::updateFixed()
  {
    // Search through each z-order map to find theEntityID provided
    std::map<const Uint32, std::deque<IEntity*> >::iterator anIter;
    anIter = mEntities.begin();
    while(anIter != mEntities.end())
    {
      std::deque<IEntity*>::iterator anQueue = anIter->second.begin();
      while(anQueue != anIter->second.end())
      {
        // Get the IEntity address first
        GQE::IEntity* anEntity = *anQueue;

        // Increment the IEntity iterator second
        anQueue++;

        // Get the ActionGroup stored as a property in the IEntity object
        ActionGroup anActionGroup = anEntity->mProperties.get<ActionGroup>("Actions");

        // Call the ActionGroup DoActions method with the given IEntity
        anActionGroup.doActions(anEntity);
      } // while(anQueue != anIter->second.end())

      // Increment map iterator
      anIter++;
    } //while(anIter != mEntities.end())
  }

  void ActionSystem::updateVariable(float theElaspedTime)
  {
  }

  void ActionSystem::draw()
  {
  }

  void ActionSystem::handleInit(IEntity* theEntity)
  {
    // Do nothing
  }

  void ActionSystem::handleCleanup(IEntity* theEntity)
  {
    // Do nothing
  }

  void ActionSystem::eraseAction(std::map<const typeActionID, IAction*>::iterator theActionIter)
  {
    // Get our IEntity reference first
    IAction* anEntity = theActionIter->second;

    // First remove the IAction from our list
    mActions.erase(theActionIter);
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

