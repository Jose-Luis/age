////////////////////////////////////////////////////////////
/// \mainpage
///
/// \section welcome Welcome
/// Welcome to the official AGE documentation. Here you will find a detailed
/// view of all the AGE <a href="./annotated.htm">classes</a>, as well as source
/// <a href="./files.htm">files</a>. <br>
/// If you are looking for tutorials, you can visit the official website
/// at <a href="http://code.google.com/p/age/">code.google.com/p/age/</a>.
///
/// \section example Short example
/// Here is a short example, to show you how simple it is to use AGE :
///
/// \code
///
///#include <assert.h>
///#include <stddef.h>
///#include <AGE/Core.hpp>
///
///int main(int argc, char* argv[])
///{
///  // Default anExitCode to a specific value
///  int anExitCode = AGE::StatusNoError;
///
///  // Create our action application.
///  AGE::App* anApp = new(std::nothrow) AGE::app();
///  assert(NULL != anApp && "main() Can't create Application");
/// 
///  // Process command line arguments
///  anApp->processArguments(argc, argv);
/// 
///  // Start the action application:
///  // Initialize the action application
///  // Enter the Game Loop where the application will remain until it is shutdown
///  // Cleanup the action application
///  // Exit back to here
///  anExitCode = anApp->run();
/// 
///  // Cleanup ourselves by deleting the action application
///  delete anApp;
/// 
///  // Don't keep pointers to objects we have just deleted
///  anApp = NULL;
/// 
///  // return our exit code
///  return anExitCode;
///}
/// \endcode
////////////////////////////////////////////////////////////
