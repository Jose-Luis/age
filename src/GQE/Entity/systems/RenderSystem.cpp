/**
 * Provides the RenderSystem class for handing all entity rendering in a game.
 *
 * @file src/GQE/Entity/systems/RenderSystem.cpp
 * @author Jacob Dix
 * @date 20120611 - Initial Release
 * @date 20120616 - Adjustments for new PropertyManager class
 * @date 20120622 - Small adjustments to implementation and Handle methods
 * @date 20120623 - Improved documentation and adjusted some properties
 */
#include <SFML/Graphics.hpp>
#include <GQE/Entity/systems/RenderSystem.hpp>
#include <GQE/Entity/interfaces/IEntity.hpp>

namespace GQE
{
  RenderSystem::RenderSystem(IApp& theApp):
    ISystem("RenderSystem",theApp)
  {
  }

  RenderSystem::~RenderSystem()
  {
  }

  void RenderSystem::addProperties(IEntity* theEntity)
  {
    theEntity->mProperties.add<sf::Sprite>("Sprite",sf::Sprite());
    theEntity->mProperties.add<sf::IntRect>("rSpriteRect",sf::IntRect(0,0,0,0));
    theEntity->mProperties.add<sf::Vector2f>("vScale",sf::Vector2f(1,1));
    theEntity->mProperties.add<sf::Vector2f>("vOrigin",sf::Vector2f(0,0));
    theEntity->mProperties.add<sf::Vector2f>("vPosition",sf::Vector2f(0,0));
    theEntity->mProperties.add<float>("fRotation", 0.0f);
    theEntity->mProperties.add<bool>("bVisible", true);
  }

  void RenderSystem::handleInit(IEntity* theEntity)
  {
    // Do nothing
  }

  void RenderSystem::handleEvents(sf::Event theEvent)
  {
  }

  void RenderSystem::updateFixed()
  {
  }

  void RenderSystem::updateVariable(float theElapsedTime)
  {
  }

  void RenderSystem::draw()
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

        // See if this IEntity is visible, if so draw it now
        if(anEntity->mProperties.get<bool>("bVisible"))
        {
          // get the other RenderSystem properties now
          sf::Sprite anSprite=anEntity->mProperties.get<sf::Sprite>("Sprite");
          anSprite.setPosition(anEntity->mProperties.get<sf::Vector2f>("vPosition"));
          anSprite.setRotation(anEntity->mProperties.get<float>("fRotation"));
          anSprite.setTextureRect(anEntity->mProperties.get<sf::IntRect>("rSpriteRect"));
          anSprite.setOrigin(anEntity->mProperties.get<sf::Vector2f>("vOrigin"));
          mApp.mWindow.draw(anSprite);
        } // if(anEntity->mProperties.get<bool>("bVisible"))
      } // while(anQueue != anIter->second.end())

      // Increment map iterator
      anIter++;
    } //while(anIter != mEntities.end())
  }

  void RenderSystem::handleCleanup(IEntity* theEntity)
  {
    // Do nothing
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
