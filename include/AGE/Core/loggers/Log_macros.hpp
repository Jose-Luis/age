/**
 * Provides several macros for logging information to the global logger (see
 * AGE::gLogger) and is by several AGE Core modules.
 *
 * @file include/AGE/Core/loggers/Log_macros.hpp
 * @author Ryan Lindeman
 * @date 20110610 - Initial Release
 * @date 20120426 - Change to ILogger::GetLogger call instead of gLogger
 */
#ifndef   AGE_LOG_MACROS_HPP_INCLUDED
#define   AGE_LOG_MACROS_HPP_INCLUDED

#include <AGE/Core/Core_types.hpp>
#include <AGE/Core/loggers/FatalLogger.hpp>
#include <AGE/Core/loggers/ScopeLogger.hpp>

/**
 * Define AGE_STRIP_LOG to the minimum log level you wish to include when
 * compiling the AGE Libraries or AGE based game engines, all others will be
 * stripped by the compiler and not included in the library or executable.
 */
#ifndef AGE_STRIP_LOG
#define AGE_STRIP_LOG INFO_LEVEL
#endif

/////////////////////////////////////////////////////////////////////////////
// Logger Macros
/////////////////////////////////////////////////////////////////////////////
/**
 * LOG_ON macro will enable all logging facilities for the severity level
 * that is compiled into the code (see AGE_STRIP_LOG)
 */
#if AGE_STRIP_LOG <= FATAL_LEVEL
#define LOG_ON() \
  do { \
    if(AGE::ILogger::getLogger()) { \
      AGE::ILogger::getLogger()->setActive(true); \
    } \
  } while(false)
#else
#define LOG_ON() do {} while(false)
#endif

/**
 * LOG_OFF macro will disable all logging facilities for all severity levels
 * compiled into the code.
 */
#if AGE_STRIP_LOG <= FATAL_LEVEL
#define LOG_OFF() \
  do { \
    if(AGE::ILogger::getLogger()) { \
      AGE::ILogger::getLogger()->setActive(false); \
    } \
  } while(false)
#else
#define LOG_OFF() do {} while(false)
#endif

#if AGE_STRIP_LOG == INFO_LEVEL
/**
 * ILOGM macro defines a simple macro that will log theMessage provided at the
 * AGE::SeverityInfo level to the global Logger (see AGE::gLogger).
 * @param[in] theMessage to be logged as a const char* string.
 */
#define ILOGM(theMessage) \
  do { \
    if(AGE::ILogger::getLogger()) { \
      AGE::ILogger::getLogger()->logMessage(AGE::SeverityInfo, __FILE__, __LINE__, \
          theMessage); \
    } \
  } while(false)
/**
 * ILOG macro defines a simple macro that will log any << following the macro
 * at the AGE::SeverityInfo level to the global Logger (see AGE::gLogger).
 */
#define ILOG() \
  if(!AGE::ILogger::getLogger()) { \
    /* Empty */ \
  } else \
    AGE::ILogger::getLogger()->getStream(AGE::SeverityInfo, __FILE__, __LINE__)
/**
 * ILOGM_IF macro defines a simple macro that will log theMessage provided if
 * theCondition is met at the AGE::SeverityInfo level to the global Logger
 * (see AGE::gLogger).
 * @param[in] theMessage to be logged as a const char* string
 * @param[in] theCondition to be used to determine if theMessage is logged
 */
#define ILOGM_IF(theMessage, theCondition) \
  !(theCondition) ? (void) 0 : ILOGM(theMessage)
/**
 * ILOG_IF macro defines a simple macro that will log any << following this
 * macro if theCondition is met at the AGE::SeverityInfo level to the global
 * Logger (see AGE::gLogger).
 * @param[in] theCondition to be used to determine if theMessage is logged
 */
#define ILOG_IF(theCondition) \
  !(theCondition) ? (void) 0 : ILOG()
#else
#define ILOGM(theMessage) do {} while(false)
#define ILOG() if(true) { /* Empty */ } else AGE::ILogger::gNullStream
#define ILOGM_IF(theMessage, theCondition) \
  !(theCondition) ? (void) 0 : ILOGM(theMessage)
#define ILOG_IF(condition) \
  !(theCondition) ? (void) 0 : ILOG()
#endif

#if AGE_STRIP_LOG <= WARN_LEVEL
/**
 * WLOGM macro defines a simple macro that will log theMessage provided at the
 * AGE::SeverityWarn level to the global Logger (see AGE::gLogger).
 * @param[in] theMessage to be logged as a const char* string.
 */
#define WLOGM(theMessage) \
  do { \
    if(AGE::ILogger::getLogger()) { \
      AGE::ILogger::getLogger()->logMessage(AGE::SeverityWarning, __FILE__, __LINE__, \
          theMessage); \
    } \
  } while(false)

/**
 * WLOG macro defines a simple macro that will log any << following the macro
 * at the AGE::SeverityWarn level to the global Logger (see AGE::gLogger).
 */
#define WLOG() \
  if(!AGE::ILogger::getLogger()) { \
    /* Empty */ \
  } else \
    AGE::ILogger::getLogger()->getStream(AGE::SeverityWarning, __FILE__, __LINE__)

/**
 * WLOGM_IF macro defines a simple macro that will log theMessage provided if
 * theCondition is met at the AGE::SeverityWarn level to the global Logger
 * (see AGE::gLogger).
 * @param[in] theMessage to be logged as a const char* string
 * @param[in] theCondition to be used to determine if theMessage is logged
 */
#define WLOGM_IF(theMessage, theCondition) \
  !(theCondition) ? (void) 0 : WLOGM(theMessage)

/**
 * WLOG_IF macro defines a simple macro that will log any << following this
 * macro if theCondition is met at the AGE::SeverityWarn level to the global
 * Logger (see AGE::gLogger).
 * @param[in] theCondition to be used to determine if theMessage is logged
 */
#define WLOG_IF(theCondition) \
  !(theCondition) ? (void) 0 : WLOG()

#else
#define WLOGM(theMessage) do {} while(false)
#define WLOG() if(true) { /* Empty */ } else AGE::ILogger::gNullStream
#define WLOGM_IF(theMessage, theCondition) \
  !(theCondition) ? (void) 0 : WLOGM(theMessage)
#define WLOG_IF(condition) \
  !(theCondition) ? (void) 0 : WLOG()
#endif

#if AGE_STRIP_LOG <= ERROR_LEVEL
/**
 * ELOGM macro defines a simple macro that will log theMessage provided at the
 * AGE::SeverityError level to the global Logger (see AGE::gLogger).
 * @param[in] theMessage to be logged as a const char* string.
 */
#define ELOGM(theMessage) \
  do { \
    if(AGE::ILogger::getLogger()) { \
      AGE::ILogger::getLogger()->logMessage(AGE::SeverityError, __FILE__, __LINE__, \
          theMessage); \
    } \
  } while(false)

/**
 * ELOG macro defines a simple macro that will log any << following the macro
 * at the AGE::SeverityError level to the global Logger (see AGE::gLogger).
 */
#define ELOG() \
  if(!AGE::ILogger::getLogger()) { \
    /* Empty */ \
  } else \
    AGE::ILogger::getLogger()->getStream(AGE::SeverityError, __FILE__, __LINE__)

/**
 * ELOGM_IF macro defines a simple macro that will log theMessage provided if
 * theCondition is met at the AGE::SeverityError level to the global Logger
 * (see AGE::gLogger).
 * @param[in] theMessage to be logged as a const char* string
 * @param[in] theCondition to be used to determine if theMessage is logged
 */
#define ELOGM_IF(theMessage, theCondition) \
  !(theCondition) ? (void) 0 : ELOGM(theMessage)

/**
 * ELOG_IF macro defines a simple macro that will log any << following this
 * macro if theCondition is met at the AGE::SeverityError level to the global
 * Logger (see AGE::gLogger).
 * @param[in] theCondition to be used to determine if theMessage is logged
 */
#define ELOG_IF(theCondition) \
  !(theCondition) ? (void) 0 : ELOG()

#else
#define ELOGM(theMessage) do {} while(false)
#define ELOG() if(true) { /* Empty */ } else AGE::ILogger::gNullStream
#define ELOGM_IF(theMessage, theCondition) \
  !(theCondition) ? (void) 0 : ELOGM(theMessage)
#define ELOG_IF(condition) \
  !(theCondition) ? (void) 0 : ELOG()
#endif

#if AGE_STRIP_LOG <= FATAL_LEVEL
/**
 * FLOGM macro defines a simple macro that will log theMessage provided at the
 * AGE::SeverityFatal level to the global Logger (see AGE::gLogger).  After
 * logging theMessage it will either call the Quit method using the global App
 * (see AGE::gApp) pointer or call exit if the global App pointer is NULL (see
 * ILogger::FatalShutdown).
 * @param[in] theMessage to be logged as a const char* string.
 * @param[in] theExitCode value to use when exiting the application
 */
#define FLOGM(theMessage, theExitCode) \
  do { \
    AGE::FatalLogger(AGE::SeverityFatal, __FILE__, __LINE__, theExitCode) \
      .logMessage(theMessage); \
  } while(false)

/**
 * FLOG macro defines a simple macro that will log any << following the macro
 * at the AGE::SeverityFatal level to the global Logger (see AGE::gLogger).
 * After logging theMessage it will either call the Quit method using the
 * global App (see AGE::gApp) pointer or call exit if the global App pointer is
 * NULL (see ILogger::FatalShutdown).
 * @param[in] theExitCode value to use when exiting the application
 */
#define FLOG(theExitCode) \
  if(!AGE::ILogger::getLogger()) { \
    /* Empty */ \
  } else \
    AGE::FatalLogger(AGE::SeverityFatal, __FILE__, __LINE__, theExitCode) \
      .getStream()

/**
 * FLOGM_IF macro defines a simple macro that will log theMessage provided if
 * theCondition is met at the AGE::SeverityFatal level to the global Logger
 * (see AGE::gLogger).  After logging theMessage it will either call the Quit
 * method using the global App (see AGE::gApp) pointer or call exit if the
 * global App pointer is NULL (see ILogger::FatalShutdown).
 * @param[in] theMessage to be logged as a const char* string
 * @param[in] theCondition to be used to determine if theMessage is logged
 * @param[in] theExitCode value to use when exiting the application
 */
#define FLOGM_IF(theMessage, theCondition, theExitCode) \
  !(theCondition) ? (void) 0 : FLOGM(theMessage, theExitCode)

/**
 * FLOG_IF macro defines a simple macro that will log any << following this
 * macro if theCondition is met at the AGE::SeverityFatal level to the global
 * Logger (see AGE::gLogger).  After logging theMessage it will either call the
 * Quit method using the global App (see AGE::gApp) pointer or call exit if the
 * global App pointer is NULL (see ILogger::FatalShutdown).
 * @param[in] theCondition to be used to determine if theMessage is logged
 * @param[in] theExitCode value to use when exiting the application
 */
#define FLOG_IF(theCondition, theExitCode) \
  !(theCondition) ? (void) 0 : FLOG(theExitCode)

#else
#define FLOGM(theMessage, theExitCode) do {} while(false)
#define FLOG() if(true) { /* Empty */ } else AGE::ILogger::gNullStream
#define FLOGM_IF(theMessage, theCondition, theExitCode) \
  !(theCondition) ? (void) 0 : FLOGM(theMessage, theExitCode)
#define FLOG_IF(theCondition, theExitCode) \
  !(theCondition) ? (void) 0 : FLOG(theExitCode)
#endif

#if AGE_STRIP_LOG == INFO_LEVEL
/**
 * SLOG macro defines a simple macro that will log the Entry and Exit of the
 * scope (which might be a method or function) along with any << following
 * the macro when the scope macro variable (_gql_ + theName) created goes out
 * of scope. The messages and arguments are logged at theSeverity (see
 * AGE::SeverityType) level provided to the global Logger (see AGE::gLogger).
 * The Exit log entry will use the line number of the last SLOGR message used.
 * If no SLOGR macros were used the line number will be the same as the Entry
 * log message used.
 * @param[in] theScopeName (not in quotes!) to use for the variable created
 * @param[in] theSeverity to use when logging Entry and Exit of scope
 */
#define SLOG(theScopeName, theSeverity) \
  AGE::ScopeLogger _gql_##theScopeName(theSeverity, __FILE__, __LINE__, \
    #theScopeName); \
  _gql_##theScopeName.getStream()

/**
 * SLOG macro defines a simple macro that will log the Entry and Exit of the
 * scope (which might be a method or function) along with any << following
 * the macro when the scope macro variable (_gql_ + theName) created goes out
 * of scope. The messages and arguments are logged at theSeverity (see
 * AGE::SeverityType) level provided to the global Logger (see AGE::gLogger).
 * The Exit log entry will use the line number of the last SLOGR message used.
 * If no SLOGR macros were used the line number will be the same as the Entry
 * log message used.
 * @param[in] theScopeName (not in quotes!) to use for the variable created
 * @param[in] theSeverity to use when logging Entry and Exit of scope
 * @param[in] theExitCode value to use when exiting the application
 */
#define SLOGE(theScopeName, theSeverity, theExitCode) \
  AGE::ScopeLogger _gql_##theScopeName(theSeverity, __FILE__, __LINE__, \
    theExitCode, #theScopeName); \
  _gql_##theScopeName.getStream()

/**
 * SLOGR macro defines a simple macro that will allow the logging of all <<
 * following the SLOGR macro which is likely the return value(s) from a method
 * or function scope.  Every call to SLOGR will update the severity level,
 * source file, and source line number used when the Exit entry is logged by
 * the SLOG created ScopeLogger class, this is to help determine from the log
 * which return path a scope may have used on Exit. If the severity level of
 * SLOG or the last SLOGR is equal to AGE::SeverityFatal then the FatalShutdown
 * method (see ILogger) will be called after the Exit entry log message is
 * recorded in the ScopeLogger deconstructor. All the << values following the
 * SLOGR macro will be logged at theSeverity (see AGE::SeverityType) level
 * provided to the global Logger (see AGE::gLogger).
 * @param[in] theScopeName (not in quotes!) to use for the variable reference
 * @param[in] theSeverity to use when logging Entry and Exit of scope
 */
#define SLOGR(theScopeName, theSeverity) \
  if(!AGE::ILogger::getLogger()) { \
    /* Empty */ \
  } else \
    _gql_##theScopeName.getStream(theSeverity, __FILE__, __LINE__)

/**
 * SLOGR macro defines a simple macro that will allow the logging of all <<
 * following the SLOGR macro which is likely the return value(s) from a method
 * or function scope.  Every call to SLOGR will update the severity level,
 * source file, and source line number used when the Exit entry is logged by
 * the SLOG created ScopeLogger class, this is to help determine from the log
 * which return path a scope may have used on Exit. If the severity level of
 * SLOG or the last SLOGR is equal to AGE::SeverityFatal then the FatalShutdown
 * method (see ILogger) will be called after the Exit entry log message is
 * recorded in the ScopeLogger deconstructor. All the << values following the
 * SLOGR macro will be logged at theSeverity (see AGE::SeverityType) level
 * provided to the global Logger (see AGE::gLogger).
 * @param[in] theScopeName (not in quotes!) to use for the variable reference
 * @param[in] theSeverity to use when logging Entry and Exit of scope
 * @param[in] theExitCode value to use when exiting the application
 */
#define SLOGRE(theScopeName, theSeverity, theExitCode) \
  if(!AGE::ILogger::getLogger()) { \
    /* Empty */ \
  } else \
    _gql_##theScopeName.getStream(theSeverity, __FILE__, __LINE__, theExitCode)

#else
#define SLOG(theScopeName, theSeverity) \
  if(true) { /* Empty */ } else AGE::ILogger::gNullStream
#define SLOGE(theScopeName, theSeverity, theExitCode) \
  if(true) { /* Empty */ } else AGE::ILogger::gNullStream
#define SLOGR(theScopeName, theSeverity) \
  if(true) { /* Empty */ } else AGE::ILogger::gNullStream
#define SLOGRE(theScopeName, theSeverity, theExitCode) \
  if(true) { /* Empty */ } else AGE::ILogger::gNullStream
#endif

#endif // AGE_LOG_MACROS_HPP_INCLUDED

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
