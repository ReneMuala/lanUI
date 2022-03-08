//
//  Settings.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 10/10/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#ifndef LANUI_SETTINGS_HPP
#define LANUI_SETTINGS_HPP

namespace LUI
{
extern const char * BUNDLE_NAME;

extern const unsigned int MAX_PROGRAM_WINDOWS;

/// Derprecated, new implementation in Utilities/PathResolver.hpp
extern const char * DEFAULT_FONTS_PATH;

/* Renderer thread sleep time for WM mode (ms) */
extern const unsigned WM_RENDERER_THREAD_SLEEP_TIME;
}

#define LUIVersion "LUI 0.5 Deprecated"


  #define LANUI_DEBUG_MODE
/*  ^^^^^^^^^^^^^^^^^^^^^^^
 
 USAGE: Uncomment the line above to enable debug mode (BEFORE BUILDING LANUI)

 DESCRIPTION: DEBUG MODE
 
 -------------------------------------------------------------------------------

 */

#ifdef LANUI_DEBUG_MODE

//#define LANUI_DEBUG_SEMAPHORE_WARN_WHEN_INVALID_HOLD
//#define LANUI_DEBUG_BSEMAPHORE_WARN_WHEN_INVALID_HOLD

/*  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 
 USAGE: enable debug mode and uncomment the lines above
 
 DESCRIPTION: LANUI will show warnings whenever the program trys to hold a busy object.
 
 -------------------------------------------------------------------------------
 
 */

//  #define LANUI_DEBUG_SEMAPHORE_WARN_WHEN_INVALID_LEAVE
//  #define LANUI_DEBUG_BSEMAPHORE_WARN_WHEN_INVALID_LEAVE
/*  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 
 USAGE: enable debug mode and uncomment the lines above
 
 DESCRIPTION: LANUI will show warnings whenever the program trys to leave an object that isn't busy.
 
 -------------------------------------------------------------------------------
 
 */

//#define LANUI_DEBUG_SEMAPHORE_ERROR_WHEN_INVALID_HOLD
//#define LANUI_DEBUG_BSEMAPHORE_ERROR_WHEN_INVALID_HOLD
/*  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 
 USAGE: enable debug mode and uncomment the line above
 
 DESCRIPTION: LANUI will show errors and stop the execution whenever the program trys to hold a busy object.
 
 -------------------------------------------------------------------------------
 
 */

//  #define LANUI_DEBUG_SEMAPHORE_ERROR_WHEN_INVALID_LEAVE
//  #define LANUI_DEBUG_BSEMAPHORE_ERROR_WHEN_INVALID_LEAVE
/*  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 
 USAGE: enable debug mode and uncomment the line above
 
 DESCRIPTION: LANUI will show errors and stop the execution whenever the program trys to leave an object that isn't busy.
 
 -------------------------------------------------------------------------------
 
 */



//  #define LANUI_DEBUG_PRINT_RENDERER_LOCK_AND_UNLOCK_EVENTS
/*  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 
 USAGE: enable debug mode and uncomment the line above
 
 DESCRIPTION: LANUI will print the new size of renderer draw area whenever the renderer is locked or unlocked.
 
 -------------------------------------------------------------------------------
 
 */

//  #define LANUI_DEBUG_PRINT_OBJECT_TEXT_ERRORS
/*  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 
 USAGE: enable debug mode and uncomment the line above
 
 DESCRIPTION: LANUI will print show messages whenever an error related to text objects occurs.
 
 -------------------------------------------------------------------------------
 
 */


#endif

/// Lambda body to be passed as a function argument
#define CallbackExpr(x) [&]{x}

#define LUI_ALLOW_MESSAGE_LOGS

#define LUI_ALLOW_WARNING_LOGS

#define LUI_ALLOW_ERROR_LOGS

#endif /* LANUI_SETTINGS_HPP */
