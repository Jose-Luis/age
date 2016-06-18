/**
 * Provides the FileLogger class that provides logging support to a file for
 * all AGE classes in the AGE namespace in the AGE core library.
 *
 * @file src/AGE/Core/loggers/FileLogger.cpp
 * @author Ryan Lindeman
 * @date 20110524 - Initial Release
 * @date 20120504 - Fix segfault caused by SLOG taking over gInstance
 */

#include <AGE/Core/loggers/FileLogger.hpp>

namespace AGE
{
  FileLogger::FileLogger(const char* theFilename, bool theDefault) :
    ILogger(theDefault)
  {
    mFileStream.open(theFilename);
    if(mFileStream.is_open())
    {
      logMessage(SeverityInfo, __FILE__, __LINE__, "FileLogger::ctor()");
    }
  }

  FileLogger::~FileLogger()
  {
    if(mFileStream.is_open())
    {
      logMessage(SeverityInfo, __FILE__, __LINE__, "FileLogger::dtor()");
      mFileStream.close();
    }
  }

  std::ostream& FileLogger::getStream(void)
  {
    std::ostream* anResult = &gNullStream;
    if(mFileStream.is_open() && isActive())
    {
      anResult = &mFileStream;
    }
    return *anResult;
  }

  std::ostream& FileLogger::getStream(SeverityType theSeverity,
      const char* theSourceFile, int theSourceLine, int theExitCode)
  {
    std::ostream* anResult = &gNullStream;
    if(mFileStream.is_open() && isActive())
    {
      anResult = &mFileStream;
      writeTag(mFileStream, theSeverity, theSourceFile, theSourceLine);
    }
    return *anResult;
  }

  void FileLogger::logMessage(const char* theMessage)
  {
    if(mFileStream.is_open() && isActive())
    {
      mFileStream << theMessage << std::endl;
    }
  }

  void FileLogger::logMessage(SeverityType theSeverity,
      const char* theSourceFile, int theSourceLine,
      const char* theMessage)
  {
    if(mFileStream.is_open() && isActive())
    {
      writeTag(mFileStream, theSeverity, theSourceFile, theSourceLine);
      mFileStream << theMessage << std::endl;
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
