/**
 * Provides the App class in the GQE namespace which is responsible for
 * providing the App base class implementation used in the GQE core library.
 *
 * @file src/GQE/Core/classes/App.cpp
 * @author Ryan Lindeman
 * @date 20100705 - Initial Release
 * @date 20110106 - Added ConfigReader for loading window settings in PreInit
 * @date 20110118 - Fixed compiler problems with ConfigReader
 * @date 20110125 - Drop use of non-standard _getcwd for now.
 * @date 20110125 - IState::HandleCleanup is now called from StateManager
 * @date 20110127 - Moved to GQE Core library and src directory
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
 * @date 20120512 - Add new Init hooks for derived classes and changed name to IApp
 * @date 20120609 - Default to 20 UPS, 20 FPS, and windowed mode and added new
 *                  improved gameloop.
 * @date 20120622 - Remove setting of show value of StatManager to false
 * @date 20120630 - Add new SetGraphicRange and CalculateGraphicRange methods
 * @date 20120702 - Call new IState::Cleanup method during game loop
 */

#include <assert.h>
#include <GQE/Core/assets/ConfigAsset.hpp>
#include <GQE/Core/assets/ConfigHandler.hpp>
#include <GQE/Core/assets/FontHandler.hpp>
#include <GQE/Core/assets/ImageHandler.hpp>
#include <GQE/Core/assets/MusicHandler.hpp>
#include <GQE/Core/assets/SoundHandler.hpp>
#include <GQE/Core/classes/ConfigReader.hpp>
#include <GQE/Core/interfaces/IApp.hpp>
#include <GQE/Core/interfaces/IState.hpp>
#include <GQE/Core/loggers/Log_macros.hpp>

namespace GQE
{
  /// Default application wide settings file string
  const char* IApp::APP_SETTINGS = "resources/settings.cfg";

  /// Single instance of the most recently created App class
  IApp* IApp::gApp = NULL;

  IApp::IApp(const std::string theTitle) :
    mTitle(theTitle),
    mVideoMode(DEFAULT_VIDEO_WIDTH, DEFAULT_VIDEO_HEIGHT, DEFAULT_VIDEO_BPP),
    mWindow(),

    mContextSettings(),

    mWindowStyle(sf::Style::Close | sf::Style::Resize),
    mGraphicRange(LowRange),

    mUpdateRate((Uint32)(1000.0f / 20.0f)), // 20 updates per second

    mMaxUpdates(5)
  {
    // Save our global App pointer
    gApp = this;
  }

  IApp::~IApp()
  {
    // Make sure our running flag is false
    mRunning = false;

    // Are we going out of scope? then remove our static pointer
    if(gApp == this)
    {
      gApp = NULL;
    }
  }

  IApp* IApp::getApp(void)
  {
    return gApp;
  }

  void IApp::processArguments(int argc, char* argv[])
  {
    // Handle command line arguments
    // TODO: Add handling of command line arguments
    if(argc == 1)
    {
      ILOG() << "IApp::processArguments(" << argv[0] << ") command line: (none)" << std::endl;
    }
    else
    {
      ILOG() << "IApp::processArguments(" << argv[0] << ") command line:" << std::endl;
      for(int iloop = 1; iloop<argc; iloop++)
      {
        ILOG() << "Argument" << iloop << "=(" << argv[iloop] << ")" << std::endl;
      }
    }
  }

  int IApp::run(void)
  {
    SLOG(App_Run,SeverityInfo) << std::endl;

    // First set our Running flag to true
    mRunning = true;

    // Register our App pointer with our StatManager
    mStatManager.registerApp(this);

    // Register our App pointer with our StateManager
    mStateManager.registerApp(this);

    // First register the IAssetHandler derived classes in the GQE Core library
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

    if(mExitCode < 0)
      SLOGR(App_Run,SeverityError) << "exitCode=" << mExitCode << std::endl;
    else
      SLOGR(App_Run,SeverityInfo) << "exitCode=" << mExitCode << std::endl;

    // Return the Exit Code specified by Quit or 0 of Quit was never called
    return mExitCode;
  }

  bool IApp::isRunning(void) const
  {
    // Return true if game loop is still running
    return mRunning;
  }

  void IApp::setGraphicRange(const GraphicRange theGraphicRange)
  {
    // Sanity check theGraphicRange provided
    if(theGraphicRange >= LowRange && theGraphicRange <= HighRange)
    {
      mGraphicRange = theGraphicRange;
    }
  }

  float IApp::getUpdateRate(void) const
  {
    // Return the previously set UpdateFixed game loop rate

    return (1000.0f / (float)mUpdateRate);

  }

  void IApp::setUpdateRate(float theRate)
  {
    if(200.0f >= theRate && 1.0f <= theRate)
    {
      // Make note of the fixed updates per second rate

      mUpdateRate = (Uint32)(1000.0f / theRate);

    }
  }

  void IApp::setMaxUpdates(Uint32 theMaxUpdates)
  {
    // Validate Max Updates range first
    if(200 >= theMaxUpdates && 1 <= theMaxUpdates)
    {
      // Set max updates value to theMaxUpdates value provided
      mMaxUpdates = theMaxUpdates;
    }
  }

  void IApp::quit(int theExitCode)
  {
    mExitCode = theExitCode;
    mRunning = false;
  }

  const GraphicRange IApp::calculateRange(Uint32 theHeight) const
  {
    // Default to LowRange
    GraphicRange anResult = LowRange;

    // Do we fall under the Medium Range category?
    if(((theHeight - 240) / 10.0) > 52.0 && ((theHeight - 240) / 10.0) <= 72.0)
    {
      // Return MidRange
      anResult = MidRange;
    }
    else if(((theHeight - 240) / 10.0) > 72.0)
    {
      // Return HighRange
      anResult = HighRange;
    }

    // Return anResult determined above or the default of LowRange
    return anResult;
  }

  void IApp::initSettingsConfig(void)
  {
    SLOG(App_InitSettingsConfig, SeverityInfo) << std::endl;
    ConfigAsset anSettingsConfig(IApp::APP_SETTINGS);
  }

  void IApp::initRenderer(void)
  {
    SLOG(App_InitRenderer, SeverityInfo) << std::endl;
    ConfigAsset anSettingsConfig(IApp::APP_SETTINGS);

    // Are we in Fullscreen mode?
    if(anSettingsConfig.getAsset().getBool("window","fullscreen",false))
    {
      mWindowStyle = sf::Style::Fullscreen;
    }


    // What size window does the user want?
    mVideoMode.width =
      anSettingsConfig.getAsset().getUint32("window","width",DEFAULT_VIDEO_WIDTH);
    mVideoMode.height =
      anSettingsConfig.getAsset().getUint32("window","height",DEFAULT_VIDEO_HEIGHT);
    mVideoMode.bitsPerPixel =
      anSettingsConfig.getAsset().getUint32("window","depth",DEFAULT_VIDEO_BPP);

    // For Fullscreen, verify valid VideoMode, otherwise revert to defaults for Fullscreen
    if(sf::Style::Fullscreen == mWindowStyle && false == mVideoMode.isValid())
    {
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

  void IApp::gameLoop(void)
  {
    SLOG(App_GameLoop, SeverityInfo) << std::endl;

    // Clock used in restricting Update loop to a fixed rate
    sf::Clock anUpdateClock;


    // Clock used in calculating the time elapsed since the last frame
    sf::Clock anFrameClock;

    // Restart/Reset our Update clock
    anUpdateClock.restart();

    // When do we need to update next (in milliseconds)?
    sf::Int32 anUpdateNext = anUpdateClock.getElapsedTime().asMilliseconds();


    // Make sure we have at least one state active
    if(mStateManager.isEmpty())
    {
      // Exit with an error since there isn't an active state
      quit(StatusAppInitFailed);
    }

    // Loop while IsRunning returns true

    while(isRunning() && mWindow.isOpen() && !mStateManager.isEmpty())

    {
      // Get the currently active state
      IState& anState = mStateManager.getActiveState();

      // Count the number of sequential UpdateFixed loop calls
      Uint32 anUpdates = 0;

      // Process any available input
      processInput(anState);

      // Make note of the current update time

      sf::Int32 anUpdateTime = anUpdateClock.getElapsedTime().asMilliseconds();


      // Process our UpdateFixed portion of the game loop
      while((anUpdateTime - anUpdateNext) >= mUpdateRate && anUpdates++ < mMaxUpdates)
      {
        // Let the current active state perform fixed updates next
        anState.updateFixed();

        // Let the StatManager perfom its updates
        mStatManager.updateFixed();

        //ILOG() << "IApp::updateFixed() anUpdates=" << anUpdates
        //  << ", anUpdateTime=" << anUpdateTime << ", anUpdateNext=" << anUpdateNext
        //  << ", mUpdateRate=" << mUpdateRate << ", anUpdateActual="

        //  << (anUpdateClock.getElapsedTime().asMilliseconds() - anUpdateTime) << std::endl;


        // Compute the next appropriate UpdateFixed time
        anUpdateNext += mUpdateRate;
      } // while((anUpdateTime - anUpdateNext) >= mUpdateRate && anUpdates <= mMaxUpdates)

      // Let the current active state perform its variable update

      // Convert to floating point value of seconds for SFML 2.0
      anState.updateVariable(anFrameClock.restart().asSeconds());


      // Let the current active state draw stuff
      anState.draw();

      // Let the StatManager perform its drawing
      mStatManager.draw();


      // Display Render window to the screen
      mWindow.display();


      // Give the state manager a chance to delete any pending states
      mStateManager.cleanup(); 
    } // while(isRunning() && !mStates.empty())
  }

  void IApp::processInput(IState& theState)
  {
    // Variable for storing the current input event to be processed
    sf::Event anEvent;


    while(mWindow.pollEvent(anEvent))

    {
      // Handle some input events and let the current state handle the rest

      // Switch on Event Type
      switch(anEvent.type)

      {
        case sf::Event::Closed:       // Window closed
          quit(StatusAppOK);
          break;
        case sf::Event::GainedFocus:  // Window gained focus
          theState.resume();
          break;
        case sf::Event::LostFocus:    // Window lost focus
          theState.pause();
          break;
        case sf::Event::Resized:      // Window resized
          break;
        default:                      // Current active state will handle
          theState.handleEvents(anEvent);
      } // switch(anEvent.Type)
    } // while(mWindow.getEvent(anEvent))
  }

  void IApp::cleanup(void)
  {
    SLOG(App_Cleanup, SeverityInfo) << std::endl;

    // Give the StatManager a chance to de-initialize
    mStatManager.deInit();

    // Close the Render window if it is still open

    if(mWindow.isOpen())

    {

      // Show the Mouse cursor
      mWindow.setMouseCursorVisible(true);

      // Close the Render window
      mWindow.close();

    }
  }

} // namespace GQE

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
