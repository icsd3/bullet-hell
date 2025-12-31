cmake_minimum_required(VERSION 3.26)

# NOTE: update executable name in .github/workflows/cmake.yml:25 when changing executable name in this file
# for now, the project name is used as the executable name
set(MAIN_PROJECT_NAME "oop")
set(MAIN_EXECUTABLE_NAME "${MAIN_PROJECT_NAME}")


project(${MAIN_PROJECT_NAME})
# set(CMAKE_PROJECT_VERSION_MAJOR 0)
# set(CMAKE_PROJECT_VERSION_MINOR 0)
# set(CMAKE_PROJECT_VERSION_PATCH 1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/Options.cmake)
include(cmake/CompilerFlags.cmake)
include(cmake/CopyHelper.cmake)

###############################################################################

# external dependencies with FetchContent

include(FetchContent)

set(FETCHCONTENT_QUIET OFF)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

set(SFML_WARNINGS_AS_ERRORS OFF)

# NOTE: Also update SFML_VERSION env var in .github/workflows/cmake.yml:33
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    # renovate: datasource=github-tags depName=SFML/SFML versioning=loose
    GIT_TAG        3.0.2
    # renovate: depName=SFML/SFML
    # GIT_TAG        0fa201c969e48ecc253581c5841ce73f44d42f49 # 3.0.2
    GIT_SHALLOW    1  # works only with branches or tags, not with arbitrary commit hashes
)

FetchContent_MakeAvailable(SFML)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
)

FetchContent_MakeAvailable(nlohmann_json)

###############################################################################

# external dependencies with find_package

find_package(Threads REQUIRED)
if(APPLE)
elseif(UNIX)
    find_package(X11)
endif()

###############################################################################

# NOTE: update executable name in .github/workflows/cmake.yml:25 when changing name here
add_executable(${MAIN_EXECUTABLE_NAME}
    main.cpp
    headers/GameLogic.h
    source/GameLogic.cpp
    headers/MainMenu.h
    source/MainMenu.cpp
    headers/Object.h
    source/Object.cpp
    headers/Entity.h
    source/Entity.cpp
    headers/Player.h
    source/Player.cpp
    headers/Weapon.h
    source/Weapon.cpp
    headers/GUI.h
    source/GUI.cpp
    headers/Projectile.h
    source/Projectile.cpp
    headers/Enemy.h
    source/Enemy.cpp
    headers/Settings.h
    source/Settings.cpp
    headers/Augment.h
    source/Augment.cpp
    headers/Level.h
    source/Level.cpp
    headers/Room.h
    source/Room.cpp
    headers/EnemyRoom.h
    source/EnemyRoom.cpp
    headers/BossRoom.h
    source/BossRoom.cpp
    headers/Minimap.h
    source/Minimap.cpp
    headers/Utils.h
    source/Utils.cpp
    headers/Door.h
    source/Door.cpp
    headers/Collider.h
    source/Collider.cpp
    headers/Gun.h
    source/Gun.cpp
    headers/ResourceManager.h
    source/ResourceManager.cpp
    headers/Exception.h
    source/Exception.cpp
    headers/Attack.h
    source/Attack.cpp
    headers/Slash.h
    source/Slash.cpp
    headers/Sword.h
    source/Sword.cpp
    headers/Beam.h
    source/Beam.cpp
    headers/GameOverMenu.h
    source/GameOverMenu.cpp
    headers/Laser.h
    source/Laser.cpp
    headers/PauseMenu.h
    source/PauseMenu.cpp
    headers/Stab.h
    source/Stab.cpp
    headers/Spear.h
    source/Spear.cpp
)

# NOTE: Add all defined targets (e.g. executables, libraries, etc. )
# NOTE: RUN_SANITIZERS is optional, if it's not present it will default to true
set_compiler_flags(RUN_SANITIZERS TRUE TARGET_NAMES ${MAIN_EXECUTABLE_NAME})
# set_compiler_flags(TARGET_NAMES ${MAIN_EXECUTABLE_NAME} ${FOO} ${BAR})
# where ${FOO} and ${BAR} represent additional executables or libraries
# you want to compile with the set compiler flags

###############################################################################

# use SYSTEM so cppcheck and clang-tidy do not report warnings from these directories
# target_include_directories(${MAIN_EXECUTABLE_NAME} SYSTEM PRIVATE ext/<SomeHppLib>/include)
target_include_directories(${MAIN_EXECUTABLE_NAME} SYSTEM PRIVATE ${SFML_SOURCE_DIR}/include ${nlohmann_json_SOURCE_DIR}/include)
target_link_directories(${MAIN_EXECUTABLE_NAME} PRIVATE ${SFML_BINARY_DIR}/lib)
target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE SFML::Graphics SFML::Window SFML::System Threads::Threads nlohmann_json::nlohmann_json)

if(APPLE)
elseif(UNIX)
    target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE X11)
endif()

###############################################################################

# copy binaries to "bin" folder; these are uploaded as artifacts on each release
# DESTINATION_DIR is set as "bin" in cmake/Options.cmake:6
install(TARGETS ${MAIN_EXECUTABLE_NAME} DESTINATION ${DESTINATION_DIR})
if(APPLE)
    install(FILES launcher.command DESTINATION ${DESTINATION_DIR})
endif()

copy_files(FILES tastatura.txt DIRECTORY textures fonts json COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})