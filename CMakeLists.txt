cmake_minimum_required(VERSION 3.16)
set(PROJECT_NAME  "A2_ECS_Asteroid")

project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(${PROJECT_NAME} src/main.cpp
        src/data/vector2.h
        src/data/color.h
        src/game/components/components.h
        src/game/components/projectile_tag.h
        src/game/components/asteroid_tag.h
        src/game/components/player_tag.h
        src/game/components/collider.h
        src/game/components/shape.h
        src/game/components/speed.h
        src/game/components/transform.h
        src/game/components/velocity.h
        src/ecs/world.h
        src/ecs/systems.h
        src/ecs/filter.h
        src/ecs/pools.h
        src/game/game.cpp
        src/game/game.h
        src/game/systems/input_system.h
        src/game/systems/draw_system.h
        src/game/systems/spawn_asteroid_system.h
        src/game/systems/spawn_player_system.h
        src/data/color.h
        src/ini/ini_config.cpp
        src/ini/ini_config.h
        src/utils/utils.h
        src/utils/utils.h
        src/game/systems/move_player_system.h
        src/game/systems/shoot_player_system.h
        src/game/config/config.cpp
        src/game/config/config.h
        src/game/systems/cooldown_tick_system.h

)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
        VERBATIM)
endif()

install(TARGETS ${PROJECT_NAME})