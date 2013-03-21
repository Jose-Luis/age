/**
 * Provides the SpaceDots GameState example in the GQE library.
 *
 * @file src/GameState.cpp
 * @author Ryan Lindeman
 * @date 20120323 - Initial Release
 * @date 20120421 - Make sure SFML 2 doesn't use its default font since it will crash on exit
 * @date 20120512 - Use new RAII Asset style
 */
#include "GameState.hpp"
#include <GQE/Core/interfaces/IApp.hpp>

GameState::GameState(GQE::IApp& theApp) :
  GQE::IState("Game",theApp),
  mWinFont("resources/WinFont.ttf"),
  mBackground("resources/Background.png"),
  mEmptyHorizontal("resources/EmptyHorizontal.png"),
  mEmptyVertical("resources/EmptyVertical.png"),
  mEmptySquare("resources/EmptySquare.png"),
  mBlueHorizontal("resources/BlueHorizontal.png"),
  mBlueVertical("resources/BlueVertical.png"),
  mBlueSquare("resources/BlueSquare.png"),
  mBlueWinner("resources/BlueWinner.png"),
  mRedHorizontal("resources/RedHorizontal.png"),
  mRedVertical("resources/RedVertical.png"),
  mRedSquare("resources/RedSquare.png"),
  mRedWinner("resources/RedWinner.png"),
  mCorner("resources/Corner.png"),
  mRedGain("resources/RedGain.ogg"),
  mRedWin("resources/RedWin.ogg"),
  mBlueGain("resources/BlueGain.ogg"),
  mBlueWin("resources/BlueWin.ogg"),
  mLightsaber("resources/Lightsaber.ogg"),
  mSelectedCol(1),
  mSelectedRow(1),
  mCurrentPlayer(0),
  mWinnerText(NULL)
{
}

GameState::~GameState(void)
{
  delete mWinnerText;
  mWinnerText = NULL;
}

void GameState::doInit(void)
{
  // First call our base class implementation
  IState::doInit();

  // Load all assets now
  mApp.mAssetManager.loadAllAssets();

  // Assign our background image sprite texture

  mBackgroundSprite.setTexture(mBackground.getAsset());



  mRedGainSound.setBuffer(mRedGain.getAsset());
  mRedGainSound.setVolume(25.0f);
  mRedWinSound.setBuffer(mRedWin.getAsset());
  mBlueGainSound.setBuffer(mBlueGain.getAsset());
  mBlueGainSound.setVolume(25.0f);
  mBlueWinSound.setBuffer(mBlueWin.getAsset());
  mLightsaberSound.setBuffer(mLightsaber.getAsset());
  mLightsaberSound.setVolume(10.0f);



  // Setup winner text color as Yellow
  mWinnerText = new sf::Text("", mWinFont.getAsset(), 30);
  mWinnerText->setColor(sf::Color::Yellow);


  // Call ReInit to reset the board
  reInit();

  // Make sure our update loop is only called 30 times per second
  mApp.setUpdateRate(30.0f);
}

void GameState::reInit(void)
{
  // Reset our board egdes
  for(GQE::Uint8 row = 0; row < 19; row++)
  {
    for(GQE::Uint8 col = 0; col < 19; col++)
    {
      // Make this spot available
      mBoardPlayer[row][col] = 0;

      // Row: Even && Col: Even == Corner
      if((row % 2) == 0 && (col % 2) == 0)
      {

        mBoardSprites[row][col].setTexture(mCorner.getAsset());
        mBoardSprites[row][col].setPosition(2.0f+(col/2.0f)*(12.0f+75.0f),
            2.0f+(row/2.0f)*(9.0f+56.0f));

        // Make this corner unavailable
        mBoardPlayer[row][col] = 3;
      }
      // Row: Odd && Col: Odd == Square
      else if((row % 2) == 1 && (col % 2) == 1)
      {

        mBoardSprites[row][col].setTexture(mEmptySquare.getAsset());
        mBoardSprites[row][col].setPosition(2.0f+12.0f+((col-1)/2.0f)*(12.0f+75.0f),
            2.0f+9.0f+((row-1)/2.0f)*(9.0f+56.0f));

      }
      // Row: Even && Col: Odd == Horizontal Edge
      else if((row % 2) == 0 && (col % 2) == 1)
      {

        mBoardSprites[row][col].setTexture(mEmptyHorizontal.getAsset());
        mBoardSprites[row][col].setPosition(2.0f+12.0f+((col-1)/2.0f)*(12.0f+75.0f),
            2.0f+(row/2.0f)*(9.0f+56.0f));

      }
      // Row: Odd && Col: Even == Vertical Edge
      else if((row % 2) == 1 && (col % 2) == 0)
      {

        mBoardSprites[row][col].setTexture(mEmptyVertical.getAsset());
        mBoardSprites[row][col].setPosition(2.0f+(col/2.0f)*(12.0f+75.0f),
            2.0f+9.0f+((row-1)/2.0f)*(9.0f+56.0f));

      }
    }
  }

  // Set current player to player 1
  mCurrentPlayer = 1;

  // Set selected row and column to known values
  mSelectedCol = 1;
  mSelectedRow = 1;

  // Reset our winner text

  mWinnerText->setString("");

}

void GameState::handleEvents(sf::Event theEvent)
{
  // Exit program if Escape key is pressed

    if((theEvent.type == sf::Event::KeyReleased) && (theEvent.key.code == sf::Keyboard::Escape))

    {
      // Signal the application to exit
      mApp.quit(GQE::StatusAppOK);
    }

  // Update selected edge if mouse moved

    if(theEvent.type == sf::Event::MouseMoved)

    {
      updateSelected(theEvent);
    }


    if(theEvent.type == sf::Event::MouseButtonReleased)

    {
      // If current player is 0 then the last game ended, start a new game
      if(0 == mCurrentPlayer)
      {
        // Reinitialize the board and start a new game
        reInit();
      }
      else
      {
        // Player has selected an edge
        selectEdge();
      }
    }
}

void GameState::updateSelected(sf::Event theEvent)
{

  // Determine which square they clicked on
  GQE::Uint16 col = (GQE::Uint16)(theEvent.mouseMove.x / ((12+75)/2));
  GQE::Uint16 row = (GQE::Uint16)(theEvent.mouseMove.y / ((9+56)/2));


  // Update board selection image if different
  if((mBoardPlayer[mSelectedRow][mSelectedCol] == 0 &&
        mBoardPlayer[row][col] == 0) &&
      (row != mSelectedRow || col != mSelectedCol))
  {
    // Clear previous selected edge image
    // Row: Even && Col: Odd == Horizontal Edge
    if((mSelectedRow % 2) == 0 && (mSelectedCol % 2) == 1)
    {

      mBoardSprites[mSelectedRow][mSelectedCol].setTexture(
          mEmptyHorizontal.getAsset());

    }
    // Row: Odd && Col: Even == Vertical Edge
    else if((mSelectedRow % 2) == 1 && (mSelectedCol % 2) == 0)
    {

      mBoardSprites[mSelectedRow][mSelectedCol].setTexture(
          mEmptyVertical.getAsset());

    }
    else
    {
      /* Corner or square, do nothing */
    }

    // Set new row and column image
    // Row: Even && Col: Odd == Horizontal Edge
    if((row % 2) == 0 && (col % 2) == 1)
    {
      if(mCurrentPlayer == 1)
      {

        mBoardSprites[row][col].setTexture(
            mBlueHorizontal.getAsset());

      }
      else
      {

        mBoardSprites[row][col].setTexture(
            mRedHorizontal.getAsset());

      }
    }
    // Row: Odd && Col: Even == Vertical Edge
    else if((row % 2) == 1 && (col % 2) == 0)
    {
      if(mCurrentPlayer == 1)
      {

        mBoardSprites[row][col].setTexture(
            mBlueVertical.getAsset());

      }
      else
      {

        mBoardSprites[row][col].setTexture(
            mRedVertical.getAsset());

      }
    }
    else
    {
      /* Corner or square, do nothing */
    }

    // Update our mSelectedRow and mSelectedCol values
    mSelectedRow = row;
    mSelectedCol = col;
  }
}

void GameState::selectEdge(void)
{
  // Row: Even && Col: Odd == Horizontal Edge
  if((mSelectedRow % 2) == 0 && (mSelectedCol % 2) == 1)
  {
    // Mark this edge as selected
    mBoardPlayer[mSelectedRow][mSelectedCol] = 1;

    if(mCurrentPlayer == 1)
    {

      mBoardSprites[mSelectedRow][mSelectedCol].setTexture(
          mBlueHorizontal.getAsset());

    }
    else
    {

      mBoardSprites[mSelectedRow][mSelectedCol].setTexture(
          mRedHorizontal.getAsset());

    }


    // Only play if not already playing a sound effect
    if(sf::Sound::Playing != mLightsaberSound.getStatus())

    {
      mLightsaberSound.play();

    }


  }
  // Row: Odd && Col: Even == Vertical Edge
  else if((mSelectedRow % 2) == 1 && (mSelectedCol % 2) == 0)
  {
    // Mark this edge as selected
    mBoardPlayer[mSelectedRow][mSelectedCol] = 1;

    if(mCurrentPlayer == 1)
    {

      mBoardSprites[mSelectedRow][mSelectedCol].setTexture(
          mBlueVertical.getAsset());

    }
    else
    {

      mBoardSprites[mSelectedRow][mSelectedCol].setTexture(
          mRedVertical.getAsset());

    }


    // Only play if not already playing a sound effect
    if(sf::Sound::Playing != mLightsaberSound.getStatus())
    {
      mLightsaberSound.play();
    }



  }
  else
  {
    /* Corner or square, return to caller */
    return;
  }

  // Find all completed squares and look for end game
  bool anChangePlayers = true;
  bool anGameOver = true;
  GQE::Uint8 anScore[2] = {0,0};
  for(GQE::Uint8 row = 1; row < 18; row+=2)
  {
    for(GQE::Uint8 col = 1; col < 18; col+=2)
    {
      if(mBoardPlayer[row][col] == 0)
      {
        if(mBoardPlayer[row-1][col] == 1 &&
            mBoardPlayer[row][col-1] == 1 &&
            mBoardPlayer[row+1][col] == 1 &&
            mBoardPlayer[row][col+1] == 1)
        {
          // Update image for this square
          if(mCurrentPlayer == 1)
          {

            mBoardSprites[row][col].setTexture(
                mBlueSquare.getAsset());

          }
          else
          {

            mBoardSprites[row][col].setTexture(
                mRedSquare.getAsset());

          }

          // Assign this square to the current player
          mBoardPlayer[row][col] = mCurrentPlayer;

          // Add this square to the current players score
          anScore[mCurrentPlayer - 1]++;

          // Give this player another turn
          anChangePlayers = false;
        }
        else
        {
          // Game is not over
          anGameOver = false;

          // Reset selected row and column to this row and column
          mSelectedRow = row;
          mSelectedCol = col;
        }
      }
      else
      {
        // Count the score for each player
        anScore[mBoardPlayer[row][col] - 1]++;
      }
    }
  }

  // Is game not over? then see if we need to switch players
  if(!anGameOver)
  {
    // Didn't complete a square? then change players now
    if(anChangePlayers)
    {
      if(mCurrentPlayer == 1)
      {
        mCurrentPlayer = 2;
      }
      else
      {
        mCurrentPlayer = 1;
      }
    }
    else
    {
    // Play appropriate sound
      if(mCurrentPlayer == 1)
      {
        // Only play if not currently playing this sound effect
        if(sf::Sound::Playing != mBlueGainSound.getStatus())
        {
          mBlueGainSound.play();
        }
      }
      else
      {
        // Only play if not currently playing this sound effect
        if(sf::Sound::Playing != mRedGainSound.getStatus())
        {
          mRedGainSound.play();
        }
      }
    }
  }
  else
  {
    // Determine which theme to play at end of game
    if(anScore[0] > anScore[1])
    {


      // Set correct winner image to be displayed

      mBlueWinSound.play();
      mWinnerSprite.setTexture(mBlueWinner.getAsset());
      mWinnerSprite.setPosition(0.0f,0.0f);

    }
    else
    {


      // Set correct winner image to be displayed

      mRedWinSound.play();
      mWinnerSprite.setTexture(mRedWinner.getAsset());
      mWinnerSprite.setPosition(0.0f,0.0f);

    }


    mWinnerText->setString("Click button to play again!");

    // Setup winner text in middle of screen
    mWinnerText->setPosition(110.0f,540.0f);


    // Switch to no player (wait until mouse clicks to reset game)
    mCurrentPlayer = 0;
  }
}

void GameState::updateFixed(void)
{
}

void GameState::updateVariable(float theElapsedTime)
{
}

void GameState::draw(void)
{
  // Draw our Background

  mApp.mWindow.draw(mBackgroundSprite);


  // Draw our Board
  for(GQE::Uint8 row = 0; row < 19; row++)
  {
    for(GQE::Uint8 col = 0; col < 19; col++)
    {
      // Draw our Board

      mApp.mWindow.draw(mBoardSprites[row][col]);

    }
  }

  // Show winner image
  if(mCurrentPlayer == 0)
  {

    mApp.mWindow.draw(mWinnerSprite);

  }


  // Draw winner text
  mApp.mWindow.draw(*mWinnerText);


}

void GameState::handleCleanup(void)
{
}

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
