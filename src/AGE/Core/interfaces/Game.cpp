/**
 * Provides the App class in the AGE namespace which is responsible for
 * providing the App base class implementation used in the AGE core library.
 *
 * @file src/AGE/Core/classes/App.cpp
 * @author Ryan Lindeman
 * @date 20100705 - Initial Release
 * @date 20110106 - Added ConfigReader for loading window settings in PreInit
 * @date 20110118 - Fixed compiler problems with ConfigReader
 * @date 20110125 - Drop use of non-standard _getcwd for now.
 * @date 20110125 - IState::HandleCleanup is now called from StateManager
 * @date 20110127 - Moved to AGE Core library and src directory
 * @date 20110128 - Add Get/SetUpdateRate methods for changing game rate speed
 * @date 20110128 - Added new StatManager class for collecting game statistics
 * @date 20110218 - Change to system include style
 * @date 20110331 - Removed direct.h include as it is no longer needed
 * @date 20110611 - Convert logging to new Log macros and added gApp pointer
 * @date 20110625 - Added UpdateVariable and changed Update to UpdateFixed
 * @date 20110627 - Removed extra ; from namespace
 * @date 20110704 - Removed Init method (moved StatManager.DoInit to Run method)
 * @date 20110831 - Support new SFML2 snapshot changes
 * @date 20120211 - Support new SFML2 snapshot changes
 * @date 20120322 - Support new SFML2 snapshot changes
 * @date 20120426 - Convert from pointer to address for anState in Loop method.
 * @date 20120512 - Add new Init hooks for derived classes and changed name to Game
 * @date 20120609 - Default to 20 UPS, 20 FPS, and windowed mode and added new
 *                  improved gameloop.
 * @date 20120622 - Remove setting of show value of StatManager to false
 * @date 20120630 - Add new SetGraphicRange and CalculateGraphicRange methods
 * @date 20120702 - Call new IState::Cleanup method during game loop
 */

#include <assert.h>
#include <AGE/Core/assets/ConfigAsset.hpp>
#include <AGE/Core/assets/ConfigHandler.hpp>
#include <AGE/Core/assets/FontHandler.hpp>
#include <AGE/Core/assets/ImageHandler.hpp>
#include <AGE/Core/assets/MusicHandler.hpp>
#include <AGE/Core/assets/SoundHandler.hpp>
#include <AGE/Core/classes/ConfigReader.hpp>
#include <AGE/Core/interfaces/Game.hpp>
#include <AGE/Core/interfaces/IState.hpp>
#include <AGE/Core/loggers/Log_macros.hpp>

namespace AGE {
   /// Default application wide settings file string
   const char* Game::APP_SETTINGS = "resources/settings.cfg";

   /// Single instance of the most recently created App class
   Game* Game::gApp = NULL;

   Game::Game(const std::string theTitle) :
   mTitle(theTitle),
   mVideoMode(DEFAULT_VIDEO_WIDTH, DEFAULT_VIDEO_HEIGHT, DEFAULT_VIDEO_BPP),
   mWindow(),
   mContextSettings(),
   mWindowStyle(sf::Style::Close | sf::Style::Resize),
   mGraphicRange(LowRange),
   mUpdateRate((Uint32) (1000.0f / 20.0f)), // 20 updates per second
   mMaxUpdates(5)
   {
      gApp = this;
   }

   Game::~Game()
   {
      mRunning = false;

      if (gApp == this) {
         gApp = NULL;
      }
   }

   Game* Game::getApp(void)
   {
      return gApp;
   }

   void Game::processArguments(int argc, char* argv[])
   {
      // Handle command line arguments
      // TODO: Add handling of command line arguments
      if (argc == 1) {
         ILOG() << "Game::processArguments(" << argv[0] << ") command line: (none)" << std::endl;
      } else {
         ILOG() << "Game::processArguments(" << argv[0] << ") command line:" << std::endl;
         for (int iloop = 1; iloop < argc; iloop++) {
            ILOG() << "Argument" << iloop << "=(" << argv[iloop] << ")" << std::endl;
         }
      }
   }

   int Game::run(void)
   {
      SLOG(App_Run, SeverityInfo) << std::endl;

      // First set our Running flag to true
      mRunning = true;

      // Register our App pointer with our StatManager
      mStatManager.registerApp(this);

      // Register our App pointer with our StateManager
      mStateManager.registerApp(this);

      // First register the IAssetHandler derived classes in the AGE Core library
      mAssetManager.registerHandler(new(std::nothrow) ConfigHandler());
      mAssetManager.registerHandler(new(std::nothrow) FontHandler());
      mAssetManager.registerHandler(new(std::nothrow) ImageHandler());
      mAssetManager.registerHandler(new(std::nothrow) MusicHandler());
      mAssetManager.registerHandler(new(std::nothrow) SoundHandler());

      // Give derived class a time to register custom IAssetHandler classes
      initAssetHandlers();

      // Attempt to open the application wide settings.cfg file as a ConfigAsset
      // registered under the ID of "resources/settings.cfg"
      initSettingsConfig();

      // Try to open the Renderer window to display graphics
      initRenderer();

      // Give the derived application a chance to register a IScreenFactory class
      // to provide IScreen derived classes (previously known as IState derived
      // classes) as requested.
      initScreenFactory();

      // Give the StatManager a chance to initialize
      mStatManager.doInit();

      // GameLoop if Running flag is still true
      gameLoop();

      // Cleanup our application
      handleCleanup();

      // Perform our own internal Cleanup
      cleanup();

      // Make sure our Running flag is set to false before exiting
      mRunning = false;

      if (mExitCode < 0)
         SLOGR(App_Run, SeverityError) << "exitCode=" << mExitCode << std::endl;
      else
         SLOGR(App_Run, SeverityInfo) << "exitCode=" << mExitCode << std::endl;

      // Return the Exit Code specified by Quit or 0 of Quit was never called
      return mExitCode;
   }

   bool Game::isRunning(void) const
   {
      // Return true if game loop is still running
      return mRunning;
   }

   void Game::setGraphicRange(const GraphicRange theGraphicRange)
   {
      // Sanity check theGraphicRange provided
      if (theGraphicRange >= LowRange && theGraphicRange <= HighRange) {
         mGraphicRange = theGraphicRange;
      }
   }

   float Game::getUpdateRate(void) const
   {
      return(1000.0f / (float) mUpdateRate);

   }

   void Game::setUpdateRate(float theRate)
   {
      if (200.0f >= theRate && 1.0f <= theRate) {
         mUpdateRate = (Uint32) (1000.0f / theRate);
      }
   }

   void Game::quit(int theExitCode)
   {
      mExitCode = theExitCode;
      mRunning = false;
   }

   const GraphicRange Game::calculateRange(Uint32 theHeight) const
   {
      // Default to LowRange
      GraphicRange anResult = LowRange;

      // Do we fall under the Medium Range category?
      if (((theHeight - 240) / 10.0) > 52.0 && ((theHeight - 240) / 10.0) <= 72.0) {
         // Return MidRange
         anResult = MidRange;
      } else if (((theHeight - 240) / 10.0) > 72.0) {
         // Return HighRange
         anResult = HighRange;
      }

      // Return anResult determined above or the default of LowRange
      return anResult;
   }

   void Game::initSettingsConfig(void)
   {
      SLOG(App_InitSettingsConfig, SeverityInfo) << std::endl;
      ConfigAsset anSettingsConfig(Game::APP_SETTINGS);
   }

   void Game::initRenderer(void)
   {
      SLOG(App_InitRenderer, SeverityInfo) << std::endl;
      ConfigAsset anSettingsConfig(Game::APP_SETTINGS);

      // Are we in Fullscreen mode?
      if (anSettingsConfig.getAsset().getBool("window", "fullscreen", false)) {
         mWindowStyle = sf::Style::Fullscreen;
      }


      // What size window does the user want?
      mVideoMode.width =
              anSettingsConfig.getAsset().getUint32("window", "width", DEFAULT_VIDEO_WIDTH);
      mVideoMode.height =
              anSettingsConfig.getAsset().getUint32("window", "height", DEFAULT_VIDEO_HEIGHT);
      mVideoMode.bitsPerPixel =
              anSettingsConfig.getAsset().getUint32("window", "depth", DEFAULT_VIDEO_BPP);

      // For Fullscreen, verify valid VideoMode, otherwise revert to defaults for Fullscreen
      if (sf::Style::Fullscreen == mWindowStyle && false == mVideoMode.isValid()) {
         mVideoMode.width = DEFAULT_VIDEO_WIDTH;
         mVideoMode.height = DEFAULT_VIDEO_HEIGHT;
         mVideoMode.bitsPerPixel = DEFAULT_VIDEO_BPP;
      }

      // Calculate and set GraphicRange value
      setGraphicRange(calculateRange(mVideoMode.height));

      // Create a RenderWindow object using VideoMode object above
      mWindow.create(mVideoMode, mTitle, mWindowStyle, mContextSettings);

      // Use Vertical Sync
      mWindow.setVerticalSyncEnabled(true);

   }

   void Game::gameLoop(void)
   {
      SLOG(App_GameLoop, SeverityInfo) << std::endl;

      sf::Clock frameClock;

      if (mStateManager.isEmpty()) {
         quit(StatusAppInitFailed);
      }

      while (isRunning() && mWindow.isOpen() && !mStateManager.isEmpty()) {

         IState& anState = mStateManager.getActiveState();

         frameClock.restart();
         while (frameClock.getElapsedTime().asMilliseconds() < mUpdateRate) {

            processInput(anState);
            anState.updateVariable(frameClock.getElapsedTime().asMilliseconds());
         }

         anState.draw();
         mStatManager.draw();
         mWindow.display();

         mStateManager.cleanup();
      }
   }

   void Game::processInput(IState& theState)
   {
      sf::Event anEvent;


      while (mWindow.pollEvent(anEvent)) {

         switch (anEvent.type) {
         case sf::Event::Closed: // Window closed
            quit(StatusAppOK);
            break;
         case sf::Event::GainedFocus: // Window gained focus
            theState.resume();
            break;
         case sf::Event::LostFocus: // Window lost focus
            theState.pause();
            break;
         case sf::Event::Resized: // Window resized
            break;
         default: // Current active state will handle
            theState.handleEvents(anEvent);
         } // switch(anEvent.Type)
      } // while(mWindow.getEvent(anEvent))
   }

   void Game::cleanup(void)
   {
      SLOG(App_Cleanup, SeverityInfo) << std::endl;

      // Give the StatManager a chance to de-initialize
      mStatManager.deInit();

      // Close the Render window if it is still open

      if (mWindow.isOpen()) {

         // Show the Mouse cursor
         mWindow.setMouseCursorVisible(true);

         // Close the Render window
         mWindow.close();

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
