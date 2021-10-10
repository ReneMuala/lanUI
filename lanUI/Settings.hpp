//
//  Settings.hpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 10/10/21.
//  Copyright © 2021 René Descartes Domingos Muala. All rights reserved.
//

#ifndef Debug_h
#define Debug_h

//  #define LANUI_DEBUG_MODE
/*  ^^^^^^^^^^^^^^^^^^^^^^^
 
 USAGE: Uncomment the line above to enable debug mode (BEFORE BUILDING LANUI)

 DESCRITION: DEBUG MODE
 
 -------------------------------------------------------------------------------

 */

#ifdef LANUI_DEBUG_MODE

//  #define LANUI_DEBUG_SEMAPHORE_WARN_WHEN_INVALID_HOLD
/*  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 
 USAGE: enable debug mode and uncomment the line above
 
 DESCRIPTION: LANUI will show warnings whenever the program trys to hold a busy object.
 
 -------------------------------------------------------------------------------
 
 */

//  #define LANUI_DEBUG_SEMAPHORE_WARN_WHEN_INVALID_LEAVE
/*  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 
 USAGE: enable debug mode and uncomment the line above
 
 DESCRIPTION: LANUI will show warnings whenever the program trys to leave an object that isn't busy.
 
 -------------------------------------------------------------------------------
 
 */

//  #define LANUI_DEBUG_SEMAPHORE_ERROR_WHEN_INVALID_HOLD
/*  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 
 USAGE: enable debug mode and uncomment the line above
 
 DESCRIPTION: LANUI will show errors and stop the execution whenever the program trys to hold a busy object.
 
 -------------------------------------------------------------------------------
 
 */

//  #define LANUI_DEBUG_SEMAPHORE_ERROR_WHEN_INVALID_LEAVE
/*  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 
 USAGE: enable debug mode and uncomment the line above
 
 DESCRIPTION: LANUI will show errors and stop the execution whenever the program trys to leave an object that isn't busy.
 
 -------------------------------------------------------------------------------
 
 */
#endif

/// Lambda body to be passed as a function argument
#define CallbackExpr(x) [&]{x}

#define lanUIVersion "lanUI 0.2"

#endif /* Debug_h */
