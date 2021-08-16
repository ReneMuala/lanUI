
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
    ######## 1. add SDL2 include directory if needed
    set(SDL2_INCLUDE_DIR 
        "/opt/local/include/"
        # insert path here ...
        )
endif()

if(NOT SDL2_LIBRARY)
    ######## 2. add SDL2 library directory if needed
    set(SDL2_LIBRARY 
    "/opt/local/lib/libSDL2.dylib"
    # insert path here ...
    )
endif()

if(NOT SDL2_TTF_INCLUDE_DIR)
    ######## 3. add SDL2_ttf include directory if needed
    set(SDL2_TTF_INCLUDE_DIR 
        "/opt/local/include/"
        # insert path here ...
        )
endif()

 
if(NOT SDL2_TTF_LIBRARY)
    ######## 4. add SDL2_ttf library directory if needed
    set(SDL2_TTF_LIBRARY 
        "/opt/local/lib/libSDL2_ttf.dylib"
        # insert path here ...
        )
endif()

if(NOT SDL2_IMAGE_INCLUDE_DIR)
    ######## 5. add SDL2_image include directory if needed
    set(SDL2_IMAGE_INCLUDE_DIR 
        "/opt/local/include/"
        # insert path here ...
        )
endif()

if(NOT SDL2_IMAGE_LIBRARY)
    ######## 6. add SDL2_image library directory if needed
    set(SDL2_IMAGE_LIBRARY 
        "/opt/local/lib/libSDL2_image.dylib"
        # insert path here ...
        )
endif()

message("-- SDL2: UNSING (${SDL2_INCLUDE_DIR} AND ${SDL2_LIBRARY})")

message("-- SDL2TTF: UNSING (${SDL2_TTF_INCLUDE_DIR} AND ${SDL2_TTF_LIBRARY})")

message("-- SDL2IMAGE: UNSING (${SDL2_IMAGE_INCLUDE_DIR} AND ${SDL2_IMAGE_LIBRARY})")

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

set(CORE_DIR "./lanUI/Core")
set(INTERFACE_DIR "./lanUI/Interface")
set(PROJECT_DIR "./lanUI/Project")

message("-- Creating Template/lanUI.Bundle")
    file(
    COPY "./lanUI.Bundle"
    DESTINATION "./Template/")

message("-- Creating Template/lanUI")

make_directory("./Template/lanUI/")
    file(
    COPY "./LICENSE"
    DESTINATION "./Template/lanUI/../")

    file(
    COPY "./lanUI/lanUI.hpp"
    DESTINATION "./Template/lanUI/")

    file(
    COPY "./lanUI/Semaphore.hpp"
    DESTINATION "./Template/lanUI/")

make_directory("./Template/${CORE_DIR}/")
    file(
    COPY "./${CORE_DIR}/Core.hpp"
    DESTINATION "./Template/${CORE_DIR}/")

make_directory("./Template/${INTERFACE_DIR}/")

make_directory("./Template/${INTERFACE_DIR}/Button/")
    file(
    COPY "./${INTERFACE_DIR}/Button/Button.hpp"
    DESTINATION "./Template/${INTERFACE_DIR}/Button/")

make_directory("./Template/${INTERFACE_DIR}/Color/")
    file(
    COPY "./${INTERFACE_DIR}/Color/Color.hpp" 
    DESTINATION "./Template/${INTERFACE_DIR}/Color/")
    
make_directory("./Template/${INTERFACE_DIR}/Container/")
    file(
    COPY "./${INTERFACE_DIR}/Container/Container.hpp"
    DESTINATION "./Template/${INTERFACE_DIR}/Container/")

make_directory("./Template/${INTERFACE_DIR}/Font/")
    file(
    COPY "./${INTERFACE_DIR}/Font/Font.hpp"
    DESTINATION "./Template/${INTERFACE_DIR}/Font/")

make_directory("./Template/${INTERFACE_DIR}/Image/")
    file(
    COPY "./${INTERFACE_DIR}/Image/Image.hpp"
    DESTINATION "./Template/${INTERFACE_DIR}/Image/")

make_directory("./Template/${INTERFACE_DIR}/List/")
    file(
    COPY "./${INTERFACE_DIR}/List/List.hpp"
    DESTINATION "./Template/${INTERFACE_DIR}/List/")

make_directory("./Template/${INTERFACE_DIR}/Object/")
    file(
    COPY "./${INTERFACE_DIR}/Object/Object.hpp"
    DESTINATION "./Template/${INTERFACE_DIR}/Object/")

make_directory("./Template/${INTERFACE_DIR}/Spacer/")
    file(
    COPY "./${INTERFACE_DIR}/Spacer/Spacer.hpp"
    DESTINATION "./Template/${INTERFACE_DIR}/Spacer/")

make_directory("./Template/${INTERFACE_DIR}/Stack/")
    file(
    COPY "./${INTERFACE_DIR}/Stack/Stack.hpp"
    DESTINATION "./Template/${INTERFACE_DIR}/Stack/")

make_directory("./Template/${INTERFACE_DIR}/Text/")
    file(
    COPY "./${INTERFACE_DIR}/Text/Text.hpp"
    DESTINATION "./Template/${INTERFACE_DIR}/Text/")

make_directory("./Template/${INTERFACE_DIR}/View/")
    file(
    COPY "./${INTERFACE_DIR}/View/View.hpp"
    DESTINATION "./Template/${INTERFACE_DIR}/View/")

make_directory("./Template/${INTERFACE_DIR}/Window/")
    file(
    COPY "./${INTERFACE_DIR}/Window/Window.hpp"
    DESTINATION "./Template/${INTERFACE_DIR}/Window/")

make_directory("./Template/${PROJECT_DIR}/")
    file(
    COPY "./${PROJECT_DIR}/Project.hpp"
    DESTINATION "./Template/${PROJECT_DIR}/")

make_directory("./Template/${PROJECT_DIR}/CustomFonts/")
    file(
    COPY "./${PROJECT_DIR}/CustomFonts/CustomFonts.hpp"
    DESTINATION "./Template/${PROJECT_DIR}/CustomFonts/")

make_directory("./Template/${PROJECT_DIR}/CustomTextStyles/")
    file(
    COPY "./${PROJECT_DIR}/CustomTextStyles/CustomTextStyles.hpp"
    DESTINATION "./Template/${PROJECT_DIR}/CustomTextStyles/")

add_library(lanUI
    SHARED # change to STATIC if you want a static version of liblanUI
    "${CORE_DIR}/Core.cpp" 
    "${INTERFACE_DIR}/Button/Button.cpp" 
    "${INTERFACE_DIR}/Color/Color.cpp" 
    "${INTERFACE_DIR}/Font/Font.cpp"
    "${INTERFACE_DIR}/Image/Image.cpp"
    "${INTERFACE_DIR}/List/List.cpp"
    "${INTERFACE_DIR}/Object/Object.cpp" 
    "${INTERFACE_DIR}/Object/InteractiveObject.cpp" 
    "${INTERFACE_DIR}/Object/DrawableObject.cpp" 
    "${INTERFACE_DIR}/Stack/Stack.cpp"
    "${INTERFACE_DIR}/Text/Text.cpp"
    "${INTERFACE_DIR}/Window/Window.cpp"
    "${PROJECT_DIR}/CustomFonts/CustomFonts.cpp")

target_link_libraries(lanUI)
add_executable(lanUIApp "lanUI/main.cpp")
add_executable(lanUIApp2-demo "Examples/demo.cpp")
add_executable(lanUIApp3-hello_rock "Examples/hello_rock.cpp")
add_executable(lanUIApp4-click_gradient "Examples/click_gradient.cpp")
add_compile_definitions(USING_LANUI_CMAKE_BUILDER)
target_link_libraries(lanUIApp lanUI)
target_link_libraries(lanUIApp2-demo lanUI)
target_link_libraries(lanUIApp3-hello_rock lanUI)
target_link_libraries(lanUIApp4-click_gradient lanUI)

message(
    "
    ##                ###        ####     ##   ##         ##   ######## 
    ##               ## ##       ## ##    ##   ##         ##      ##    
    ##              ##   ##      ##  ##   ##   ##         ##      ##    
    ##             #########     ##   ##  ##   ##         ##      ##    
    ##            ##       ##    ##    ## ##   ##         ##      ##    
    ##########   ##         ##   ##     ####    ##########     ######## 
    ")