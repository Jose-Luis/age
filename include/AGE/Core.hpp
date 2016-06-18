/**
 * Provides the AGE namespace includes for the AGE Core engine includes and
 * variables.
 *
 * @file include/AGE/Core.hpp
 * @author Ryan Lindeman
 * @date 20100710 - Initial Release
 * @date 20110118 - Added ConfigReader and StateManager includes.
 * @date 20110127 - Changed name from AGE.hpp to Core.hpp and moved to include
 * @date 20110218 - Added ConfigAsset to Core includes
 * @date 20110906 - Added new StringUtil include file
 * @date 20120512 - Add new Asset Handler classes
 * @date 20120702 - Add new EventManager and IEvent classes
 * @date 20120720 - Moved PropertyManager to Core library from Entity library
 */
#ifndef   AGE_CORE_HPP_INCLUDED
#define   AGE_CORE_HPP_INCLUDED

/////////////////////////////////////////////////////////////////////////////
// Headers
/////////////////////////////////////////////////////////////////////////////
#include <AGE/Config.hpp>

// AGE Core includes
#include <AGE/Core/assets/ConfigAsset.hpp>
#include <AGE/Core/assets/ConfigHandler.hpp>
#include <AGE/Core/assets/FontAsset.hpp>
#include <AGE/Core/assets/FontHandler.hpp>
#include <AGE/Core/assets/ImageAsset.hpp>
#include <AGE/Core/assets/ImageHandler.hpp>
#include <AGE/Core/assets/MusicAsset.hpp>
#include <AGE/Core/assets/MusicHandler.hpp>
#include <AGE/Core/assets/SoundAsset.hpp>
#include <AGE/Core/assets/SoundHandler.hpp>
#include <AGE/Core/classes/AssetManager.hpp>
#include <AGE/Core/classes/ConfigReader.hpp>
#include <AGE/Core/classes/EventManager.hpp>
#include <AGE/Core/classes/PropertyManager.hpp>
#include <AGE/Core/classes/StatManager.hpp>
#include <AGE/Core/classes/StateManager.hpp>
#include <AGE/Core/interfaces/Game.hpp>
#include <AGE/Core/interfaces/IAssetHandler.hpp>
#include <AGE/Core/interfaces/IEvent.hpp>
#include <AGE/Core/interfaces/ILogger.hpp>
#include <AGE/Core/interfaces/IProperty.hpp>
#include <AGE/Core/interfaces/IState.hpp>
#include <AGE/Core/interfaces/TAsset.hpp>
#include <AGE/Core/interfaces/TAssetHandler.hpp>
#include <AGE/Core/interfaces/TEvent.hpp>
#include <AGE/Core/interfaces/TProperty.hpp>
#include <AGE/Core/loggers/Log_macros.hpp>
#include <AGE/Core/loggers/FatalLogger.hpp>
#include <AGE/Core/loggers/FileLogger.hpp>
#include <AGE/Core/loggers/ScopeLogger.hpp>
#include <AGE/Core/loggers/StringLogger.hpp>
#include <AGE/Core/loggers/onullstream>
#include <AGE/Core/states/SplashState.hpp>
#include <AGE/Core/utils/StringUtil.hpp>

#endif // AGE_CORE_HPP_INCLUDED

/**
 * @defgroup Core AGE Core module
 * The AGE Core module/library provides all basic classes used by all game
 * engines in the AGE collection of modules/libraries. Each game engine genre
 * will be built on top of the AGE Core module/library.
 *
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
