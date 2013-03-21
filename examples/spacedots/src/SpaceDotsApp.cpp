/**
 * Provides the SpaceDotsApp class which implements the SpaceDots example game
 * for the GQE library.
 *
 * @file src/SpaceDotsApp.cpp
 * @author Ryan Lindeman
 * @date 20120323 - Initial Release
 * @date 20120512 - Add new Init methods required by IApp base class
 */
#include "SpaceDotsApp.hpp"
#include "GameState.hpp"
#include <GQE/Core/states/SplashState.hpp>

SpaceDotsApp::SpaceDotsApp(const std::string theTitle) :
  GQE::IApp(theTitle)
{
}

SpaceDotsApp::~SpaceDotsApp()
{
}

void SpaceDotsApp::initAssetHandlers(void)
{
  // No custom asset handlers needed or provided
}

void SpaceDotsApp::initScreenFactory(void)
{
  // Add Menu State as the next active state
  mStateManager.addActiveState(new(std::nothrow) GameState(*this));
  mStateManager.addActiveState(new(std::nothrow) GQE::SplashState(*this, "Splash"));
}

void SpaceDotsApp::handleCleanup(void)
{
  // No custom cleanup needed
}
