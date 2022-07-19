# Linking with SDL2 libs to build graphics engine
find_package(SDL2 REQUIRED PATHS Dependencies/SDL)
find_package(SDL2_image REQUIRED PATHS Dependencies/SDL_image)
find_package(SDL2_ttf REQUIRED PATHS Dependencies/SDl_TTF)

target_include_directories(CMakeTarget PRIVATE "${SDL2_INCLUDE_DIRS}")
target_include_directories(CMakeTarget PRIVATE "${SDL2_IMAGE_INCLUDE_DIRS}")
target_include_directories(CMakeTarget PRIVATE "${SDL2_TTF_INCLUDE_DIRS}")

target_link_libraries(CMakeTarget ${SDL2_LIBRARIES})
target_link_libraries(CMakeTarget ${SDL2_IMAGE_LIBRARIES})
target_link_libraries(CMakeTarget ${SDL2_TTF_LIBRARIES})
