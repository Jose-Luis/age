/**
 * Provides the MovementSystem class for handing all entity movement in a game.
 *
 * @file src/GQE/Entity/systems/MovementSystem.cpp
 * @author Jacob Dix
 * @date 20120611 - Initial Release
 * @date 20120616 - Adjustments for new PropertyManager class
 * @date 20120618 - Use IEntity not Instance and changed AddPrototype to AddProperties
 * @date 20120622 - Small adjustments to implementation and Handle methods
 * @date 20120623 - Improved documentation and adjusted some properties
 * @date 20120630 - Improve ScreenWrap functionality using SpriteRect values
 */
#include <SFML/Graphics.hpp>
#include <GQE/Entity/systems/MovementSystem.hpp>
#include <GQE/Entity/interfaces/IEntity.hpp>

namespace GQE
{
  MovementSystem::MovementSystem(IApp& theApp):
    ISystem("MovementSystem",theApp)
  {
  }
  MovementSystem::~MovementSystem()
  {
  }

  void MovementSystem::addProperties(IEntity* theEntity)
  {
    theEntity->mProperties.add<sf::Vector2f>("vVelocity",sf::Vector2f(0,0));
    theEntity->mProperties.add<sf::Vector2f>("vAcceleration",sf::Vector2f(0,0));
    theEntity->mProperties.add<float>("fRotationalVelocity",0);
    theEntity->mProperties.add<float>("fRotationalAcceleration",0);
    theEntity->mProperties.add<bool>("bFixedMovement",true);
    theEntity->mProperties.add<bool>("bScreenWrap",true);
    theEntity->mProperties.add<bool>("bMovable",true);
  }

  void MovementSystem::handleInit(IEntity* theEntity)
  {
    // Do nothing
  }

  void MovementSystem::handleEvents(sf::Event theEvent)
  {
  }

  void MovementSystem::updateFixed()
  {
    // Search through each z-order map to find theEntityID provided
    std::map<const Uint32, std::deque<IEntity*> >::iterator anIter;
    anIter = mEntities.begin();
    while(anIter != mEntities.end())
    {
      std::deque<IEntity*>::iterator anQueue = anIter->second.begin();
      while(anQueue != anIter->second.end())
      {
        // get the IEntity address first
        GQE::IEntity* anEntity = *anQueue;

        // Increment the IEntity iterator second
        anQueue++;

        // Are we using fixed movement mathematics?
        if(anEntity->mProperties.get<bool>("bFixedMovement"))
        {
          // get the RenderSystem properties
          sf::Vector2f anPosition = anEntity->mProperties.get<sf::Vector2f>("vPosition");
          float anRotation = anEntity->mProperties.get<float>("fRotation");

          // get the MovementSystem properties
          sf::Vector2f anVelocity = anEntity->mProperties.get<sf::Vector2f>("vVelocity");
          sf::Vector2f anAccelleration = anEntity->mProperties.get<sf::Vector2f>("vAcceleration");
          float anRotationalVelocity = anEntity->mProperties.get<float>("fRotationalVelocity");
          float anRotationalAccelleration = anEntity->mProperties.get<float>("fRotationalAcceleration");

          // Now update the current movement properties
          anVelocity += anAccelleration;
          anPosition += anVelocity;
          anRotationalVelocity += anRotationalAccelleration;
          anRotation += anRotationalVelocity;

          // If ScreenWrap is true, account for screen wrapping
          if(anEntity->mProperties.get<bool>("bScreenWrap"))
          {
            // Call our universal HandleScreenWrap method to wrap this IEntity
            handleScreenWrap(anEntity, &anPosition);
          }

          // Now update the MovementSystem properties for this IEntity class
          anEntity->mProperties.set<sf::Vector2f>("vVelocity",anVelocity);
          anEntity->mProperties.set<float>("fRotationalVelocity",anRotationalVelocity);

          // Now update the RenderSystem properties of this IEntity class
          anEntity->mProperties.set<sf::Vector2f>("vPosition",anPosition);
          anEntity->mProperties.set<float>("fRotation",anRotation);
        } //if(anEntity->mProperties.get<bool>("bFixedMovement"))
      } // while(anQueue != anIter->second.end())

      // Increment map iterator
      anIter++;
    } //while(anIter != mEntities.end())
  }

  void MovementSystem::updateVariable(float theElapsedTime)
  {
    // Search through each z-order map to find theEntityID provided
    std::map<const Uint32, std::deque<IEntity*> >::iterator anIter;
    anIter = mEntities.begin();
    while(anIter != mEntities.end())
    {
      std::deque<IEntity*>::iterator anQueue = anIter->second.begin();
      while(anQueue != anIter->second.end())
      {
        // get the IEntity address first
        GQE::IEntity* anEntity = *anQueue;

        // Increment the IEntity iterator second
        anQueue++;

        // Are we NOT using fixed movement mathematics?
        if(anEntity->mProperties.get<bool>("bFixedMovement") == false)
        {
          // get the RenderSystem properties
          sf::Vector2f anPosition = anEntity->mProperties.get<sf::Vector2f>("vPosition");
          float anRotation = anEntity->mProperties.get<float>("fRotation");

          // get the MovementSystem properties
          sf::Vector2f anVelocity = anEntity->mProperties.get<sf::Vector2f>("vVelocity");
          sf::Vector2f anAccelleration = anEntity->mProperties.get<sf::Vector2f>("vAcceleration");
          float anRotationalVelocity = anEntity->mProperties.get<float>("fRotationalVelocity");
          float anRotationalAccelleration = anEntity->mProperties.get<float>("fRotationalAcceleration");

          // Now update the current movement properties
          anVelocity += anAccelleration * theElapsedTime;
          anPosition += anVelocity * theElapsedTime;
          anRotationalVelocity += anRotationalAccelleration * theElapsedTime;
          anRotation += anRotationalVelocity * theElapsedTime;

          // If ScreenWrap is true, account for screen wrapping
          if(anEntity->mProperties.get<bool>("bScreenWrap"))
          {
            // Call our universal HandleScreenWrap method to wrap this IEntity
            handleScreenWrap(anEntity, &anPosition);
          }

          // Now update the MovementSystem properties for this IEntity class
          anEntity->mProperties.set<sf::Vector2f>("vVelocity", anVelocity);
          anEntity->mProperties.set<float>("fRotationalVelocity", anRotationalVelocity);

          // Now update the RenderSystem properties of this IEntity class
          anEntity->mProperties.set<sf::Vector2f>("vPosition", anPosition);
          anEntity->mProperties.set<float>("fRotation", anRotation);
        } //if(anEntity->mProperties.get<bool>("bFixedMovement") == false)
      } // while(anQueue != anIter->second.end())

      // Increment map iterator
      anIter++;
    } //while(anIter != mEntities.end())
  }

  void MovementSystem::draw()
  {
  }

  void MovementSystem::handleCleanup(IEntity* theEntity)
  {
    // Do nothing
  }

  void MovementSystem::handleScreenWrap(IEntity* theEntity, sf::Vector2f* thePosition)
  {
    // get SpriteRect to see how many pixels to over shoot screen before wrapping
    sf::IntRect anSpriteRect = theEntity->mProperties.get<sf::IntRect>("rSpriteRect");

    // If current x is bigger than screen width then wrap to barely showing sprite
    if(thePosition->x > (float)mApp.mWindow.getSize().x)
    {
      thePosition->x = -(float)anSpriteRect.width;
    }
    // If current x is less than width of sprite then wrap to barley showing sprite
    else if(thePosition->x < -(float)anSpriteRect.width)
    {
      thePosition->x = (float)mApp.mWindow.getSize().x;
    }
    // If current y is bigger than screen height then wrap to barely showing sprite
    if(thePosition->y > (float)mApp.mWindow.getSize().y)
    {
      thePosition->y = -(float)anSpriteRect.height;
    }
    // If current y is less than height of sprite then wrap to barley showing sprite
    else if(thePosition->y < -(float)anSpriteRect.height)
    {
      thePosition->y = (float)mApp.mWindow.getSize().y;
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
