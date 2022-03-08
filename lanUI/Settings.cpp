//
//  Settings.cpp
//  lanUI
//
//  Created by René Descartes Domingos Muala on 08/03/22.
//  Copyright © 2022 René Descartes Domingos Muala. All rights reserved.
//

#include <stdio.h>
#include "Settings.hpp"

namespace LUI
{
const char * BUNDLE_NAME = "lanUI.Bundle";

const unsigned int MAX_PROGRAM_WINDOWS = 1+ /* mutable => */ 16;

/// Derprecated, new implementation in Utilities/PathResolver.hpp
const char * DEFAULT_FONTS_PATH = "lanUI.Bundle/System/Library/Fonts/";

/* Renderer thread sleep time for WM mode (ms) */
const unsigned WM_RENDERER_THREAD_SLEEP_TIME = 10;
}
