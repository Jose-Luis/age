/**
 * Provides the IEntity interface class for all game entities in a game.
 *
 * @file src/GQE/Entity/interfaces/IEntity.cpp
 * @author Jacob Dix
 * @date 20120423 - Initial Release
 * @date 20120609 - Move property methods to new PropertyManager class
 * @date 20120618 - Moved ID from Instance class to this base class
 * @date 20120620 - Drop ourselves from registered ISystem classes
 * @date 20120622 - Fix issues with dropping ISystem classes
 */
#include <GQE/Entity/interfaces/IEntity.hpp>
#include <GQE/Entity/interfaces/ISystem.hpp>
namespace GQE
{
  typeEntityID IEntity::mNextID = 0; // Start with 0

  IEntity::IEntity(IEntity* theFather) :
    mEntityID(useNextID()),
    mFather(theFather)
  {
    isRoot()?mOrder=0:mOrder=mFather->getOrder()+1;
    ILOG() << "IEntity::ctor(" << mEntityID << "," << mOrder << ")" << std::endl;
  }

  IEntity::~IEntity()
  {
    ILOG() << "IEntity::dtor(" << mEntityID << ")" << std::endl;

    // Make sure to drop all our systems
    dropAllChildren();
    dropAllSystems();
  }

  const typeEntityID IEntity::getID(void) const
  {
    return mEntityID;
  }

  typeEntityID IEntity::useNextID()
  {
    // Pre-increment the ID so that an ID of 0 can be considered an "invalid" ID
    return ++mNextID;
  }

  const Uint32 IEntity::getOrder(void) const
  {
    return mOrder;
  }

  void IEntity::setOrder(const Uint32 theOrder)
  {
    mOrder = theOrder;
  }

  void IEntity::addSystem(ISystem* theSystem)
  {
    if(mSystems.find(theSystem->getID()) == mSystems.end())
    {
      mSystems[theSystem->getID()] = theSystem;
    }
  }

  bool IEntity::hasSystem(const typeSystemID theSystemID) const
  {
    bool anResult=false;
    if(mSystems.find(theSystemID)!=mSystems.end())
    {
      anResult=true;
    }
    return anResult;
  }

  void IEntity::dropSystem(const typeSystemID theSystemID)
  {
    // Iterator for looping through each registered ISystem
    std::map<const typeSystemID, ISystem*>::iterator anSystemIter;

    // See if we can find theSystemID specified
    anSystemIter = mSystems.find(theSystemID);
    if(anSystemIter != mSystems.end())
    {
      // Call our EraseSystem variable
      eraseSystem(anSystemIter);
    }
    else
    {
      WLOG() << "IEntity::dropSystem(" << theSystemID
        << ") was not found!" << std::endl;
    }
  }

  void IEntity::dropAllSystems(void)
  {
    // Make sure we drop ourselves from all registered ISystem classes
    std::map<const typeSystemID, ISystem*>::iterator anSystemIter;

    // Start at the beginning of the list of ISystem classes
    anSystemIter = mSystems.begin();
    while(anSystemIter != mSystems.end())
    {
      eraseSystem(anSystemIter++);
    }

    // Last of all clear our list of systems
    mSystems.clear();
  }

  void IEntity::eraseSystem(std::map<const typeSystemID, ISystem*>::iterator theSystemIter)
  {
    // Get our ISystem reference first
    ISystem* anSystem = theSystemIter->second;

    // First remove the ISystem from our list
    mSystems.erase(theSystemIter);

    // Now use our ISystem reference to remove any IEntity references
    if(anSystem->hasEntity(getID()))
    {
      // Cause ISystem to drop our reference
      anSystem->dropEntity(getID());
    }
  }


  void IEntity::addChild(std::string theChildName, IEntity* theChild)
  {
      dropChild(theChildName);
      Uint32 aFatherOrder = getOrder();
      theChild->setOrder(aFatherOrder++);
      theChild->setFather(this);
      mChildren.insert(std::pair<std::string,IEntity*>(theChildName,theChild));
  }

  void IEntity::dropChild(std::string theChildName)
  {
      std::map<std::string,IEntity*>::iterator thePossibleChild = mChildren.find(theChildName);

      if(thePossibleChild != mChildren.end())
      {
          thePossibleChild->second->dropAllSystems();
      }
  }

  void IEntity::dropAllChildren()
  {
    for(auto aChild: mChildren)
    {
        aChild.second->dropAllSystems();
    }
  }

  void IEntity::setFather(IEntity* theFather)
  {
      mFather = theFather;
  }

  IEntity* IEntity::getFather()
    {return mFather;}

  bool IEntity::isRoot()
  { return mFather == nullptr;}

  IEntity* IEntity::getRoot()
  {
      IEntity* aRoot = this;
      while (!aRoot->isRoot()) aRoot->getFather();
      return aRoot;
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
