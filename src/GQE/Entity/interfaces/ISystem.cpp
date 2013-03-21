/**
 * The ISystem interface class used for all derived ISystem entities that
 * can be added to an IEntity class.
 *
 * @file src/GQE/Entity/interfaces/ISystem.cpp
 * @author Jacob Dix
 * @date 20120423 - Initial Release
 * @date 20120618 - Use IEntity not Instance and changed AddPrototype to AddProperties
 * @date 20120620 - Drop ourselves from registered IEntity classes
 * @date 20120622 - Fix issues with dropping IEntity classes
 * @date 20120707 - Fix pure-virtual crash which calls HandleCleanup in dtor
 */
#include <GQE/Entity/interfaces/ISystem.hpp>
#include <GQE/Entity/interfaces/IEntity.hpp>
#include <GQE/Core/loggers/Log_macros.hpp>

namespace GQE
{
  ISystem::ISystem(const typeSystemID theSystemID, IApp& theApp) :
    mApp(theApp),
    mSystemID(theSystemID)
  {
    ILOG() << "ISystem::ctor(" << mSystemID << ")" << std::endl;
  }

  ISystem::~ISystem()
  {
    ILOG() << "ISystem::dtor(" << mSystemID << ")" << std::endl;

    // Make sure to drop all our entities
    dropAllEntities();
  }

  const typeSystemID ISystem::getID(void) const
  {
    return mSystemID;
  }

  const typeEntityID ISystem::addEntity(IEntity* theEntity)
  {
    typeEntityID anResult = 0;

    // Make sure the caller didn't give us a bad pointer
    if(theEntity != NULL)
    {
      // This will be true if the IEntity was added
      bool anAdded = false;

      // See if this IEntity is already in this z-order class
      std::deque<IEntity*>::iterator anFind;
      anFind = std::find(mEntities[theEntity->getOrder()].begin(),
        mEntities[theEntity->getOrder()].end(), theEntity);

        // If IEntity wasn't found in this z-order class then add it now
      if(anFind == mEntities[theEntity->getOrder()].end())
      {
        // Add theEntity provided to this deque
        mEntities[theEntity->getOrder()].push_back(theEntity);

        // Make sure this entity has the correct properties added for this system
        addProperties(theEntity);

        // Add this system to this entity
        theEntity->addSystem(this);

        // Perform any custom Initialization for this new IEntity before adding it
        handleInit(theEntity);

        // Return the ID of this IEntity as a result
        anResult = theEntity->getID();
      }
      else
      {
        WLOG() << "ISystem::addEntity(" << theEntity->getID()
          << ") Entity already exists!" << std::endl;
      }
    }
    else
    {
      ELOG() << "ISystem::addEntity() Null Entity pointer provided!" << std::endl;
    }

    // Return the ID of this IEntity class or 0 if something went wrong
    return anResult;
  }

  bool ISystem::hasEntity(const typeEntityID theEntityID) const
  {
    bool anResult = false;

    // Search through each z-order map to find theEntityID provided
    std::map<const Uint32, std::deque<IEntity*> >::const_iterator anIter;
    anIter = mEntities.begin();
    while(anIter != mEntities.end() && anResult == false)
    {
      std::deque<IEntity*>::const_iterator anFind = anIter->second.begin();
      while(anFind != anIter->second.end())
      {
        // Is this theEntityID we are looking for? then note it and return
        if(theEntityID == (*anFind)->getID())
        {
          // We found theEntityID
          anResult = true;

          // Exit while loop
          break;
        }

        // Increment find iterator
        anFind++;
      }

      // Increment map iterator
      anIter++;
    }

    // Return anResult which will be true if IEntity was found, false otherwise
    return anResult;
  }

  void ISystem::dropEntity(const typeEntityID theEntityID)
  {
    // This will be set to true if theEntityID was found and dropped
    bool anFound = false;

    // Search through each z-order map to find theEntityID provided
    std::map<const Uint32, std::deque<IEntity*> >::iterator anIter;
    anIter = mEntities.begin();
    while(anIter != mEntities.end() && anFound == false)
    {
      std::deque<IEntity*>::iterator anFind = anIter->second.begin();
      while(anFind != anIter->second.end())
      {
        // Is this theEntityID we are looking for? then note it and return
        if(theEntityID == (*anFind)->getID())
        {
          // Erase the IEntity from our deque
          anFind = eraseEntity(anFind);

          // Set anFound to true
          anFound = true;

          // Exit while loop
          break;
        }

        // Increment find iterator
        anFind++;
      }

      // Increment map iterator
      anIter++;
    }
  }

  void ISystem::dropAllEntities(void)
  {
    // Search through each z-order map to find theEntityID provided
    std::map<const Uint32, std::deque<IEntity*> >::iterator anIter;
    anIter = mEntities.begin();
    while(anIter != mEntities.end())
    {
      std::deque<IEntity*>::iterator anFind = anIter->second.begin();
      while(anFind != anIter->second.end())
      {
        // Erase the IEntity from our deque
        anFind = eraseEntity(anFind);
      }

      // Increment map iterator
      anIter++;
    }

    // Last of all clear our list of deques
    mEntities.clear();
  }

  void ISystem::handleCleanup(IEntity* theEntity)
  {
    // Do nothing
  }

  std::deque<IEntity*>::iterator ISystem::eraseEntity(std::deque<IEntity*>::iterator theEntityIter)
  {
    std::deque<IEntity*>::iterator anResult;

    // Get our IEntity reference first
    IEntity* anEntity = (*theEntityIter);

    anResult = mEntities[anEntity->getOrder()].erase(theEntityIter);

    // Now handle any last minute cleanup for this IEntity
    handleCleanup(anEntity);

    // Now use our IEntity reference to remove any ISystem references
    if(anEntity->hasSystem(getID()))
    {
      // Cause IEntity to drop our reference
      anEntity->dropSystem(getID());
    }

    // Return anResult iterator to caller which might be end()
    return anResult;
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
