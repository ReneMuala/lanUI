
    ##                ###        ####     ##   ##         ##   ######## 
    ##               ## ##       ## ##    ##   ##         ##      ##    
    ##              ##   ##      ##  ##   ##   ##         ##      ##    
    ##             #########     ##   ##  ##   ##         ##      ##    
    ##            ##       ##    ##    ## ##   ##         ##      ##    
    ##########   ##         ##   ##     ####    ##########     ######## 

# WElCOME
# follow this steps to build a fresh copy of laUnI
# FIRST THINGS FIRST: lanUi depends on 
# -> libSDL2
# -> libSDL2_ttf
# -> libSDL2_image
# Make sure to install those dependeces berofe trying to build lanUI

cmake_minimum_required(VERSION 3.7)

find_package(SDL2 REQUIRED)

if(NOT SDL2_INCLUDE_DIR)
    # 1. add SDL2 include directory if needed
    set(SDL2_INCLUDE_DIR 
        "/opt/local/include/"
        # insert path here ...
        )
endif()

if(NOT SDL2_LIBRARY)
    # 2. add SDL2 library directory if needed
    set(SDL2_LIBRARY 
    "/opt/local/lib/libSDL2.dylib"
    # insert path here ...
    )
endif()

if(NOT SDL2_TTF_INCLUDE_DIR)
    # 3. add SDL2_ttf include directory if needed
    set(SDL2_TTF_INCLUDE_DIR 
        "/opt/local/include/"
        # insert path here ...
        )
endif()

 
if(NOT SDL2_TTF_LIBRARY)
    # 4. add SDL2_ttf library directory if needed
    set(SDL2_TTF_LIBRARY 
        "/opt/local/lib/libSDL2_ttf.dylib"
        # insert path here ...
        )
endif()

if(NOT SDL2_IMAGE_INCLUDE_DIR)
    # 5. add SDL2_image include directory if needed
    set(SDL2_IMAGE_INCLUDE_DIR 
        "/opt/local/include/"
        # insert path here ...
        )
endif()

if(NOT SDL2_IMAGE_LIBRARY)
    # 6. add SDL2_image library directory if needed
    set(SDL2_IMAGE_LIBRARY 
        "/opt/local/lib/libSDL2_image.dylib"
        # insert path here ...
        )
endif()

message("-- SDL2: UNSING (${SDL2_INCLUDE_DIR} :: ${SDL2_LIBRARY})")

message("-- SDL2TTF: UNSING (${SDL2_TTF_INCLUDE_DIR} :: ${SDL2_TTF_LIBRARY})")

message("-- SDL2IMAGE: UNSING (${SDL2_IMAGE_INCLUDE_DIR} :: ${SDL2_IMAGE_LIBRARY})")

if(NOT SDL2_INCLUDE_DIR OR NOT SDL2_LIBRARY)
    message("-- [CONFIG ERROR] Please provide a valid SDL2 path")
endif()

if(NOT SDL2_TTF_INCLUDE_DIR OR NOT SDL2_TTF_LIBRARY)
    message("-- [CONFIG ERROR] Please provide a valid SDL2_ttf path")
endif()

if(NOT SDL2_IMAGE_INCLUDE_DIR OR NOT SDL2_IMAGE_LIBRARY)
    message("-- [CONFIG ERROR] Please provide a valid SDL2_image path")
endif()


# if(NOT MINGW)
#     set(CMAKE_FIND_LIBRARY_PREFIXES "lib") 
# else()
#     set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib") 
# endif()

# if(APPLE)
#     set(CMAKE_FIND_LIBRARY_SUFFIXES ".dylib;.so;.a")
# endif()

# if(NOT APPLE AND NOT MINGW)
#     set(CMAKE_FIND_LIBRARY_SUFFIXES ".so;.a")
# endif()



message(
    "
    ##                ###        ####     ##   ##         ##   ######## 
    ##               ## ##       ## ##    ##   ##         ##      ##    
    ##              ##   ##      ##  ##   ##   ##         ##      ##    
    ##             #########     ##   ##  ##   ##         ##      ##    
    ##            ##       ##    ##    ## ##   ##         ##      ##    
    ##########   ##         ##   ##     ####    ##########     ######## 
    ")

project(lanUI)

include_directories(SYSTEM 
    ${SDL2_INCLUDE_DIR}
    ${SDL2_TTF_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
)

add_compile_options(-std=c++17)

add_link_options(${SDL2_LIBRARY})
add_link_options(${SDL2_TTF_LIBRARY})
add_link_options(${SDL2_IMAGE_LIBRARY})

add_library(lanUI
    SHARED # change to STATIC if you want a static version of liblanUI
    "lanUI/Core/Core.cpp" 
    "lanUI/Interface/Button/Button.cpp" 
    "lanUI/Interface/Color/Color.cpp" 
    "lanUI/Interface/Font/Font.cpp"
    "lanUI/Interface/Image/Image.cpp"
    "lanUI/Interface/List/List.cpp"
    "lanUI/Interface/Object/Object.cpp" 
    "lanUI/Interface/Object/InteractiveObject.cpp" 
    "lanUI/Interface/Object/DrawableObject.cpp" 
    "lanUI/Interface/Stack/Stack.cpp"
    "lanUI/Interface/Text/Text.cpp"
    "lanUI/Interface/Window/Window.cpp"
    "lanUI/Project/CustomFonts/CustomFonts.cpp")


target_link_libraries(lanUI)

add_executable(lanUIApp "lanUI/main.cpp")

target_link_libraries(lanUIApp lanUI)
