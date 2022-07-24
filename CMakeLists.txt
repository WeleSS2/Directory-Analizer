# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.

# Setting up Cmake into at least 3.12
cmake_minimum_required (VERSION 3.12...3.23)

project ("Directory Analizer" VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

option(GTESTS_ENABLED "GTest Switch" OFF)

if(GTESTS_ENABLED)
    configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
    if(result)
      message(FATAL_ERROR "CMake step for googletest failed: ${result}")
    endif()
    execute_process(COMMAND ${CMAKE_COMMAND} --build .
      RESULT_VARIABLE result
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
    if(result)
      message(FATAL_ERROR "Build step for googletest failed: ${result}")
    endif()
    
    # Prevent overriding the parent project's compiler/linker
    # settings on Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    
    # Add googletest directly to our build. This defines
    # the gtest and gtest_main targets.
    add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                     ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                     EXCLUDE_FROM_ALL)
    
    # The gtest/gtest_main targets carry header search path
    # dependencies automatically when using CMake 2.8.11 or
    # later. Otherwise we have to add them here ourselves.
    if (CMAKE_VERSION VERSION_LESS 2.8.11)
      include_directories("${gtest_SOURCE_DIR}/include")
    endif()

    add_definitions(-DGTESTS__ENABLED)
endif()

# Target named just as a CMakeTarget, it's not a public app so i use default name.
add_executable (CMakeTarget "${PROJECT_SOURCE_DIR}/Directory Analizer/main.cpp")


if(GTESTS_ENABLED)
    # Link for GTests libraries
    target_link_libraries(CMakeTarget gtest_main)
endif()

# Include sub-projects and directories
add_subdirectory ("Directory Analizer")

# Copy neccesary files automatically to out (Create list)
# Support both 32 and 64 bit builds
if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
  set( LIST_NEEDED_DLL
    "${PROJECT_SOURCE_DIR}/Directory Analizer/Dependencies/SDL/lib/x64/SDL2.dll"
    "${PROJECT_SOURCE_DIR}/Directory Analizer/Dependencies/SDL_image/lib/x64/SDL2_image.dll"
    "${PROJECT_SOURCE_DIR}/Directory Analizer/Dependencies/SDL_image/lib/x64/libpng16-16.dll"
    "${PROJECT_SOURCE_DIR}/Directory Analizer/Dependencies/SDL_image/lib/x64/libjpeg-9.dll"
    "${PROJECT_SOURCE_DIR}/Directory Analizer/Dependencies/SDL_image/lib/x64/libtiff-5.dll"
    "${PROJECT_SOURCE_DIR}/Directory Analizer/Dependencies/SDL_image/lib/x64/libwebp-7.dll"
    "${PROJECT_SOURCE_DIR}/Directory Analizer/Dependencies/SDL_TTF/lib/x64/SDL2_ttf.dll"
    "${PROJECT_SOURCE_DIR}/Directory Analizer/Dependencies/SDL_TTF/lib/x64/libfreetype-6.dll"
    "${PROJECT_SOURCE_DIR}/Directory Analizer/Dependencies/SDL_TTF/lib/x64/zlib1.dll"
)
else ()
  set( LIST_NEEDED_DLL
    "${PROJECT_SOURCE_DIR}/Directory Analizer/Dependencies/SDL/lib/x86/SDL2.dll"
    "${PROJECT_SOURCE_DIR}/Directory Analizer/Dependencies/SDL_image/lib/x86/SDL2_image.dll"
    "${PROJECT_SOURCE_DIR}/Directory Analizer/Dependencies/SDL_image/lib/x86/libpng16-16.dll"
    "${PROJECT_SOURCE_DIR}/Directory Analizer/Dependencies/SDL_image/lib/x86/libjpeg-9.dll"
    "${PROJECT_SOURCE_DIR}/Directory Analizer/Dependencies/SDL_image/lib/x86/libtiff-5.dll"
    "${PROJECT_SOURCE_DIR}/Directory Analizer/Dependencies/SDL_image/lib/x86/libwebp-7.dll"
    "${PROJECT_SOURCE_DIR}/Directory Analizer/Dependencies/SDL_TTF/lib/x86/SDL2_ttf.dll"
    "${PROJECT_SOURCE_DIR}/Directory Analizer/Dependencies/SDL_TTF/lib/x86/libfreetype-6.dll"
    "${PROJECT_SOURCE_DIR}/Directory Analizer/Dependencies/SDL_TTF/lib/x86/zlib1.dll"
)
endif ()


# Copy neccesary files automatically to out (Copying loop)
foreach( file_i ${LIST_NEEDED_DLL})
    add_custom_command(
    TARGET CMakeTarget
    POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    ARGS -E copy ${file_i} $<TARGET_FILE_DIR:CMakeTarget>
)
endforeach( file_i )


# Copy content from Graphics
file(GLOB GRAPHICS
    "${PROJECT_SOURCE_DIR}/Directory Analizer/Graphics/Pngs/*.png"
)
file(GLOB FONTS 
    "${PROJECT_SOURCE_DIR}/Directory Analizer/Graphics/Fonts/*.ttf"
)
file(COPY ${GRAPHICS} DESTINATION Graphics/Pngs)
file(COPY ${FONTS} DESTINATION Graphics/Fonts)