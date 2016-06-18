/**
 * Provides the ScopeLogger class that provides logging the Entry and Exit of
 * the scope where this class is defined using the information provided at
 * construction time.  The Exit scope will be logged with the line number of
 * the where the ScopeLogger class was defined.
 *
 * @file src/AGE/Core/loggers/ScopeLogger.cpp
 * @author Ryan Lindeman
 * @date 20110610 - Initial Release
 * @date 20120426 - Change to ILogger::GetLogger call instead of gLogger
 * @date 20120504 - Fix segfault caused by SLOG taking over gInstance
 * @date 20120504 - Fix segfault caused by SLOG taking over gInstance
 */

#include <AGE/Core/loggers/ScopeLogger.hpp>

namespace AGE
{
  ScopeLogger::ScopeLogger(SeverityType theSeverity,
      const char* theSourceFile, int theSourceLine,
      const char* theScopeName, int theExitCode) :
    ILogger(false, theExitCode),
    mSeverity(theSeverity),
    mSourceFile(theSourceFile),
    mSourceLine(theSourceLine),
    mScopeName(theScopeName)
  {
    // Make sure our global logger is defined before using it
    if(AGE::ILogger::getLogger())
    {
      // Log the scope Entry with theScopeName provided to the
      // global logger (see AGE::gLogger).
      AGE::ILogger::getLogger()->getStream(mSeverity, mSourceFile, mSourceLine,
          theExitCode) << "Enter(" << theScopeName << ") ";
    }
  }

  ScopeLogger::~ScopeLogger()
  {
    // Make sure our global logger is defined before using it
    if(AGE::ILogger::getLogger())
    {
      // Log the scope Exit with mScopeName provided at construction time to
      // the global logger (see AGE::gLogger).
      AGE::ILogger::getLogger()->getStream(mSeverity, mSourceFile, mSourceLine)
        << "Exit(" << mScopeName << ")" << std::endl;
    }

    // If the severity level is Fatal then call our FatalShutdown method
    if(AGE::SeverityFatal == mSeverity)
    {
      fatalShutdown();
    }
  }

  std::ostream& ScopeLogger::getStream(void)
  {
    // Make sure our global logger is defined before using it
    if(AGE::ILogger::getLogger())
    {
      // Just return whatever the global logger returns for getStream()
      return AGE::ILogger::getLogger()->getStream();
    }
    else
    {
      // If global logger doesn't exist, gNullStream will so return that
      return AGE::ILogger::gNullStream;
    }
  }

  std::ostream& ScopeLogger::getStream(SeverityType theSeverity,
      const char* theSourceFile, int theSourceLine, int theExitCode)
  {
    // Update our internal copy of the severity level, source file, and the
    // source line so our Exit entry can report the last time this method was
    // called (see SLOGR).
    mSeverity = theSeverity;
    mSourceFile = theSourceFile;
    mSourceLine = theSourceLine;

    // Make sure our global logger is defined before using it
    if(AGE::ILogger::getLogger())
    {
      // Just return whatever the global logger returns for getStream()
      return AGE::ILogger::getLogger()->getStream(theSeverity, theSourceFile,
          theSourceLine, theExitCode) << "Result(" << mScopeName << ") ";
    }
    else
    {
      // If global logger doesn't exist, gNullStream will so return that
      return AGE::ILogger::gNullStream;
    }
  }

  void ScopeLogger::logMessage(const char* theMessage)
  {
    // Make sure our global logger is defined before using it
    if(AGE::ILogger::getLogger())
    {
      // Use the global logger to log theMessage provided
      AGE::ILogger::getLogger()->logMessage(theMessage);
    }
  }

  void ScopeLogger::logMessage(SeverityType theSeverity,
      const char* theSourceFile, int theSourceLine,
      const char* theMessage)
  {
    // Update our internal copy of the severity level, source file, and the
    // source line so our Exit entry can report the last time this method was
    // called (see SLOGR).
    mSeverity = theSeverity;
    mSourceFile = theSourceFile;
    mSourceLine = theSourceLine;

    // Make sure our global logger is defined before using it
    if(AGE::ILogger::getLogger())
    {
      // Use the global logger to log theMessage and other info provided
      AGE::ILogger::getLogger()->logMessage(theSeverity, theSourceFile, theSourceLine,
          theMessage);
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
